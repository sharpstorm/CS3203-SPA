#include "PQLPatternContext.h"

#include <memory>
#include <utility>

#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"
#include "qps/clauses/pattern/WhilePatternClause.h"
#include "qps/clauses/pattern/IfPatternClause.h"
#include "qps/clauses/pattern/AssignPatternClause.h"

using std::make_unique, std::move;

void PQLPatternContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  parsePatternClause(parserState);
}

void PQLPatternContext::parsePatternClause(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  PQLQuerySynonym* synonym = parseSynonym(parserState);
  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgumentPtr left = PQLEntityRefExtractor::extract(parserState);
  parserState->expect(PQL_TOKEN_COMMA);

  extractRemainingArgs(parserState, synonym, std::move(left));
}

void PQLPatternContext::extractRemainingArgs(QueryTokenParseState *parserState,
                                             PQLQuerySynonym* synonym,
                                             ClauseArgumentPtr firstArg) {
  ExpressionArgumentPtr exprArg = extractExpression(parserState);
  PQLToken* nextToken = parserState->expect(PQL_TOKEN_COMMA,
                                            PQL_TOKEN_BRACKET_CLOSE);

  PatternClausePtr clause;
  if (nextToken->isType(PQL_TOKEN_BRACKET_CLOSE)) {
    clause = dispatchTwoArg(synonym, std::move(firstArg),
                                          std::move(exprArg));
  } else {
    parserState->expect(PQL_TOKEN_UNDERSCORE);
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);
    clause = dispatchThreeArg(synonym, std::move(exprArg));
  }

  if (clause == nullptr) {
    parserState->getQueryBuilder()->setError(QPS_PARSER_ERR_PATTERN_TYPE);
    return;
  }

  parserState->getQueryBuilder()->addPattern(move(clause));
}

PQLQuerySynonym* PQLPatternContext::parseSynonym(
    QueryTokenParseState *parserState) {
  PQLSynonymName synName = parserState->expectSynName()->getData();
  return parserState->getQueryBuilder()->accessSynonym(synName);
}

PatternClausePtr PQLPatternContext::dispatchTwoArg(
    PQLQuerySynonym* synonym,
    ClauseArgumentPtr firstArg,
    ExpressionArgumentPtr secondArg) {
  if (synonym == nullptr) {
    return nullptr;
  }

  if (synonym->isType(PQL_SYN_TYPE_IF) && secondArg->isWildcard()) {
    return make_unique<IfPatternClause>(*synonym);
  }

  if (synonym->isType(PQL_SYN_TYPE_ASSIGN)) {
    return make_unique<AssignPatternClause>(
        *synonym, move(firstArg), move(secondArg));
  }

  return nullptr;
}

PatternClausePtr PQLPatternContext::dispatchThreeArg(
    PQLQuerySynonym* synonym,
    ExpressionArgumentPtr secondArg) {
  if (!secondArg->isWildcard()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  if (synonym == nullptr || !synonym->isType(PQL_SYN_TYPE_WHILE)) {
    return nullptr;
  }

  return make_unique<WhilePatternClause>(*synonym);
}

ExpressionArgumentPtr PQLPatternContext::extractExpression(
    QueryTokenParseState *parserState) {
  PQLToken* nextToken = parserState->expect(PQL_TOKEN_UNDERSCORE,
                                            PQL_TOKEN_LITERAL,
                                            PQL_TOKEN_STRING_LITERAL);
  if (nextToken->isCategory(PQL_LITERAL_TOKEN)) {
    return make_unique<ExpressionArgument>(nextToken->getData(), false);
  }

  if (parserState->getCurrentToken()->isCategory(PQL_LITERAL_TOKEN)) {
    nextToken = parserState->expect(PQL_TOKEN_LITERAL,
                                    PQL_TOKEN_STRING_LITERAL);
    parserState->expect(PQL_TOKEN_UNDERSCORE);
    return make_unique<ExpressionArgument>(nextToken->getData(), true);
  }

  return make_unique<ExpressionArgument>("", true);
}
