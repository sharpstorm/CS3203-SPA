#include "PQLSuchThatClauseContext.h"

using std::stoi;

PQLSuchThatClauseContext::PQLSuchThatClauseContext() {
}

ClauseArgument PQLSuchThatClauseContext::extractStatementRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLSuchThatClauseContext::extractEntityRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLSuchThatClauseContext::extractAnyRef(TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  } else if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }
  return extractCommonRef(state);
}

ClauseArgument PQLSuchThatClauseContext::extractCommonRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgument(CLAUSE_ARG_WILDCARD);
  } else if (!state->getCurrentToken()->isVarchar()) {
    throw QPSParserError("Unknown Such That clause argument");
  }

  PQLToken* synonym = expectVarchar(state);
  PQLQueryVariable* var = state->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (var == nullptr) {
    throw QPSParserError("Unknown synonym in such that clause");
  }

  return ClauseArgument(*var);
}

ClauseArgument PQLSuchThatClauseContext::extractEntity(
    TokenParseState* state) {
  state->advanceToken();
  PQLToken* entityRef = expectVarchar(state);
  expect(state, PQL_TOKEN_QUOTE);
  return ClauseArgument(entityRef->tokenData);
}

ClauseArgument PQLSuchThatClauseContext::extractStatement(
    TokenParseState* state) {
  int value = stoi(state->getCurrentToken()->tokenData);
  state->advanceToken();
  return ClauseArgument(value);
}

void PQLSuchThatClauseContext::parse(TokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
}
