#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens):
    tokenStream(tokens),
    currentStage(TOKEN_PARSE_STAGE_INIT),
    semanticErrorMsg("") {
}

bool QueryTokenParseState::isTokenStreamEnd() {
  return tokenStream.isTokenStreamEnd();
}

void QueryTokenParseState::advanceToken() {
  tokenStream.advanceToken();
}

PQLToken* QueryTokenParseState::getCurrentToken() {
  return tokenStream.getCurrentToken();
}

QueryBuilder* QueryTokenParseState::getQueryBuilder() {
  return &queryBuilder;
}

void QueryTokenParseState::advanceStage(TokenParsingStage newStage) {
  unordered_set<TokenParsingStage> allowed =
      parsingAllowedTransitions[currentStage];
  if (allowed.find(newStage) == allowed.end()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_TOKEN_SEQUENCE);
  }

  currentStage = newStage;
}

void QueryTokenParseState::setSemanticError(string error) {
  if (semanticErrorMsg != "") {
    return;
  }

  semanticErrorMsg = error;
}

bool QueryTokenParseState::hasSemanticError() {
  return semanticErrorMsg != "";
}

string QueryTokenParseState::getSemanticError() {
  return semanticErrorMsg;
}

PQLToken *QueryTokenParseState::expectVarchar() {
  PQLToken* currentToken = getCurrentToken();
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if (!currentToken->isVarchar()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  advanceToken();
  return currentToken;
}

PQLToken *QueryTokenParseState::expectSynName() {
  PQLToken* currentToken = getCurrentToken();
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  advanceToken();
  return currentToken;
}
