#include "PQLPatternParser.h"

#include <memory>
#include <utility>

#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"
#include "qps/clauses/pattern/WhilePatternClause.h"
#include "qps/clauses/pattern/IfPatternClause.h"
#include "IntermediateExpressionArgument.h"

using std::make_unique, std::unique_ptr, std::move;

PQLPatternParser::PQLPatternParser(ISourceExpressionParser *exprParser):
    assignContextParser(exprParser) {}

void PQLPatternParser::parse(QueryTokenParseState *parserState,
                             QueryBuilder* builder) {
  parserState->expect(PQL_TOKEN_PATTERN);
  unique_ptr<PQLToken> dummyAnd = make_unique<PQLToken>(PQL_TOKEN_AND);
  PQLToken* andToken = dummyAnd.get();

  while (andToken != nullptr) {
    PatternClausePtr clause = parsePatternClause(parserState, builder);
    if (clause == nullptr) {
      builder->setError(QPS_PARSER_ERR_PATTERN_TYPE);
    } else {
      builder->addPattern(std::move(clause));
    }
    andToken = parserState->tryExpect(PQL_TOKEN_AND);
  }
}

PatternClausePtr PQLPatternParser::parsePatternClause(
    QueryTokenParseState *parserState,
    QueryBuilder* builder) {
  PQLQuerySynonym* synonym = parseSynonym(parserState, builder);
  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgumentPtr left = PQLEntityRefExtractor::extract(parserState,
                                                          builder);
  parserState->expect(PQL_TOKEN_COMMA);

  return extractRemainingArgs(parserState, synonym, std::move(left));
}

PatternClausePtr PQLPatternParser::extractRemainingArgs(
    QueryTokenParseState *parserState,
    PQLQuerySynonym* synonym,
    ClauseArgumentPtr firstArg) {
  IntermediateExpressionArgumentPtr exprArg = extractExpression(parserState);
  PQLToken* nextToken = parserState->expect(PQL_TOKEN_COMMA,
                                            PQL_TOKEN_BRACKET_CLOSE);

  if (nextToken->isType(PQL_TOKEN_BRACKET_CLOSE)) {
    return dispatchTwoArg(synonym, std::move(firstArg),
                            std::move(exprArg));
  } else {
    parserState->expect(PQL_TOKEN_UNDERSCORE);
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);
    return dispatchThreeArg(synonym, std::move(firstArg),
                              std::move(exprArg));
  }
}

PQLQuerySynonym* PQLPatternParser::parseSynonym(
    QueryTokenParseState *parserState,
    QueryBuilder *builder) {
  PQLSynonymName synName = parserState->expectSynName();
  return builder->accessSynonym(synName);
}

PatternClausePtr PQLPatternParser::dispatchTwoArg(
    PQLQuerySynonym* synonym,
    ClauseArgumentPtr firstArg,
    IntermediateExpressionArgumentPtr secondArg) {
  if (synonym == nullptr) {
    return nullptr;
  }

  if (synonym->isType(PQL_SYN_TYPE_IF) && secondArg->isWildcard()) {
    return make_unique<IfPatternClause>(*synonym, std::move(firstArg));
  }

  if (synonym->isType(PQL_SYN_TYPE_ASSIGN)) {
    return assignContextParser.parse(synonym, std::move(firstArg),
                                     std::move(secondArg));
  }

  return nullptr;
}

PatternClausePtr PQLPatternParser::dispatchThreeArg(
    PQLQuerySynonym* synonym,
    ClauseArgumentPtr firstArg,
    IntermediateExpressionArgumentPtr secondArg) {
  if (!secondArg->isWildcard()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  if (synonym == nullptr || !synonym->isType(PQL_SYN_TYPE_WHILE)) {
    return nullptr;
  }

  return make_unique<WhilePatternClause>(*synonym, std::move(firstArg));
}

IntermediateExpressionArgumentPtr PQLPatternParser::extractExpression(
    QueryTokenParseState *parserState) {
  PQLToken* nextToken = parserState->expect(PQL_TOKEN_UNDERSCORE,
                                            PQL_TOKEN_LITERAL,
                                            PQL_TOKEN_STRING_LITERAL);
  if (nextToken->isCategory(PQL_LITERAL_TOKEN)) {
    return make_unique<IntermediateExpressionArgument>(
        nextToken->getData(), false);
  }

  if (parserState->isCurrentTokenCategory(PQL_LITERAL_TOKEN)) {
    nextToken = parserState->expect(PQL_TOKEN_LITERAL,
                                    PQL_TOKEN_STRING_LITERAL);
    parserState->expect(PQL_TOKEN_UNDERSCORE);
    return make_unique<IntermediateExpressionArgument>(
        nextToken->getData(), true);
  }

  return make_unique<IntermediateExpressionArgument>(
      "", true);
}