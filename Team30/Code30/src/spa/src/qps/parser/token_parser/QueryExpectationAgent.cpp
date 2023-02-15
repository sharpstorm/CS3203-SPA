#include "QueryExpectationAgent.h"

PQLToken *QueryExpectationAgent::expectVarchar() {
  PQLToken* currentToken = stream->getCurrentToken();
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if (!currentToken->isVarchar()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  stream->advanceToken();
  return currentToken;
}

PQLToken *QueryExpectationAgent::expectSynName() {
  PQLToken* currentToken = stream->getCurrentToken();
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  stream->advanceToken();
  return currentToken;
}
