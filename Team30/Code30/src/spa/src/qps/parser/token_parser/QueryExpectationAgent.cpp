#include "QueryExpectationAgent.h"

PQLSynonymName QueryExpectationAgent::expectSynName() {
  PQLToken* currentToken = stream->getCurrentToken();
  assertNotNull(currentToken);

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  stream->advanceToken();
  return currentToken->getData();
}

void QueryExpectationAgent::assertNotNull(PQLToken* token) {
  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }
}

PQLToken* QueryExpectationAgent::expectSynAttr() {
  PQLToken* currentToken = stream->getCurrentToken();
  assertNotNull(currentToken);

  if (!currentToken->isAttribute()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  stream->advanceToken();
  return currentToken;
}
