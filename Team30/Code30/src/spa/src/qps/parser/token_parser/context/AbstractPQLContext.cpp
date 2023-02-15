#include "AbstractPQLContext.h"

using std::stoi;

ClauseArgument AbstractPQLContext::extractStatementRef(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}

ClauseArgument AbstractPQLContext::extractEntityRef(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }

  return extractCommonRef(state);
}

ClauseArgument AbstractPQLContext::extractAnyRef(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  } else if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }
  return extractCommonRef(state);
}

ClauseArgument AbstractPQLContext::extractCommonRef(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgument(CLAUSE_ARG_WILDCARD);
  } else if (!state->getCurrentToken()->isVarchar()) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }

  PQLToken* synonym = state->expectVarchar();
  PQLQuerySynonym* var = state->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (var == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }

  return ClauseArgument(*var);
}

ClauseArgument AbstractPQLContext::extractEntity(
    QueryTokenParseState* state) {
  state->advanceToken();
  PQLToken* entityRef = state->expectVarchar();
  state->expect(PQL_TOKEN_QUOTE);
  return ClauseArgument(entityRef->tokenData);
}

ClauseArgument AbstractPQLContext::extractStatement(
    QueryTokenParseState* state) {
  int value = stoi(state->getCurrentToken()->tokenData);
  state->advanceToken();
  return ClauseArgument(value);
}

