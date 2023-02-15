#pragma once

#include "../PQLToken.h"
#include "token_stream/QueryTokenStream.h"
#include "qps/errors/QPSParserSyntaxError.h"

class QueryExpectationAgent {
 private:
  QueryTokenStream* stream;

 public:
  QueryExpectationAgent(QueryTokenStream* stream): stream(stream) {}

  template<typename... T>
  PQLToken *expect(T... tokenType);
  template<typename... T>
  PQLToken *expectCategory(T... tokenCategory);
  PQLToken *expectVarchar();
  PQLToken *expectSynName();
};

template<typename... PQLTokenType>
PQLToken* QueryExpectationAgent::expect(PQLTokenType... tokenType) {
  PQLToken* currentToken = stream->getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    stream->advanceToken();
    return currentToken;
  }

  throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
}

template<typename... PQLTokenCategory>
PQLToken* QueryExpectationAgent::expectCategory(
    PQLTokenCategory... tokenCategory) {
  PQLToken* currentToken = stream->getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isCategory(tokenCategory)  || ... || false)) {
    stream->advanceToken();
    return currentToken;
  }

  throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
}
