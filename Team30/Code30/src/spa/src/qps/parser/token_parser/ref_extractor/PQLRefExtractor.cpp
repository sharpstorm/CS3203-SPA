#include "PQLRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLRefExtractor::extractCommonRef(
    QueryTokenParseState* state,  QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgumentFactory::createWildcard();
  }

  // TO future HW: Somehow this place might be where to capture the attribute
  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);
  if (state->tryExpect(PQL_TOKEN_PERIOD) != nullptr) {

  }
  if (var == nullptr) {
    return nullptr;
  }

  return ClauseArgumentFactory::create(*var);
}

ClauseArgumentPtr PQLRefExtractor::extractAttributeRef(
    QueryTokenParseState* state,  QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_UNDERSCORE)) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  state->expect(PQL_TOKEN_PERIOD);
  PQLSynonymAttribute attr = extractAttribute(state, builder, var->getType());
  var->setSynonymType(attr);

  return ClauseArgumentFactory::create(*var);
}

PQLSynonymAttribute PQLRefExtractor::extractAttribute(
    QueryTokenParseState *parserState, QueryBuilder *builder,
    PQLSynonymType type) {
  PQLToken* foundToken;
  switch (type) {
    case PQL_SYN_TYPE_STMT:
    case PQL_SYN_TYPE_ASSIGN:
    case PQL_SYN_TYPE_IF:
    case PQL_SYN_TYPE_WHILE:
      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM);
      break;
    case PQL_SYN_TYPE_READ:
    case PQL_SYN_TYPE_PRINT:
      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM, PQL_TOKEN_VAR_NAME);
      break;
    case PQL_SYN_TYPE_CALL:
      foundToken = parserState->tryExpect(PQL_TOKEN_PROC_NAME, PQL_TOKEN_STMT_NUM);
      break;
    case PQL_SYN_TYPE_CONSTANT:
      foundToken = parserState->expect(PQL_TOKEN_VALUE);
      break;
    case PQL_SYN_TYPE_VARIABLE:
      foundToken = parserState->expect(PQL_TOKEN_VAR_NAME);
      break;
    case PQL_SYN_TYPE_PROCEDURE:
      foundToken = parserState->expect(PQL_TOKEN_PROC_NAME);
  }

  return getSynAttribute(foundToken);
}

PQLSynonymAttribute PQLRefExtractor::getSynAttribute(PQLToken * token) {
  switch (token->getType()) {
    case PQL_TOKEN_VAR_NAME:
      return PQLSynonymAttribute::VAR_NAME;
    case PQL_TOKEN_PROC_NAME:
      return PQLSynonymAttribute::PROC_NAME;
    case PQL_TOKEN_STMT_NUM:
      return PQLSynonymAttribute::STMT_NUM;
    case PQL_TOKEN_VALUE:
      return PQLSynonymAttribute::VALUE;
  }
}
