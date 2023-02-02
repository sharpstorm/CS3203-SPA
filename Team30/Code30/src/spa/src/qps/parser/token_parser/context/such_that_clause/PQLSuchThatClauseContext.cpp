#include "PQLSuchThatClauseContext.h"

using std::stoi;

ClauseArgument PQLSuchThatClauseContext::extractStatementRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    int value = stoi(state->getCurrentToken()->tokenData);
    return ClauseArgument(value);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLSuchThatClauseContext::extractEntityRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    PQLToken* entityRef = expectVarchar(state);
    expect(state, PQL_TOKEN_QUOTE);
    return ClauseArgument(entityRef->tokenData);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLSuchThatClauseContext::extractCommonRef(
    TokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
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

void PQLSuchThatClauseContext::parse(TokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
}
