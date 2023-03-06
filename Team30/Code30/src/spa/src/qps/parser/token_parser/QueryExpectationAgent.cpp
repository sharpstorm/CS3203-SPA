#include "QueryExpectationAgent.h"

PQLToken *QueryExpectationAgent::expectSynName() {
  PQLToken* currentToken = stream->getCurrentToken();
  assertNotNull(currentToken);

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  stream->advanceToken();
  return currentToken;
}

void QueryExpectationAgent::assertNotNull(PQLToken* token) {
  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }
}
