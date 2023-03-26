#pragma once

#include "token_stream/QueryTokenStream.h"
#include "../../errors/QPSParserSyntaxError.h"
#include "qps/common/PQLTypes.h"

class QueryTokenParseState {
 private:
  QueryTokenStream tokenStream;
  static void assertNotNull(PQLToken* token);

 public:
  explicit QueryTokenParseState(PQLTokenVector* tokens);
  bool isTokenStreamEnd();
  void advanceToken();

  PQLTokenType getCurrentTokenType();
  bool isCurrentTokenType(PQLTokenType type);
  bool isCurrentTokenCategory(PQLTokenCategory category);

  template<typename... T>
  PQLToken *expect(T... tokenType);
  template<typename... T>
  PQLToken *tryExpect(T... tokenType);
  PQLSynonymName expectSynName();
};

template<typename... PQLTokenType>
PQLToken* QueryTokenParseState::tryExpect(PQLTokenType... tokenType) {
  PQLToken* currentToken = tokenStream.getCurrentToken();
  if (currentToken == nullptr) {
    return nullptr;
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    tokenStream.advanceToken();
    return currentToken;
  }

  return nullptr;
}

template<typename... PQLTokenType>
PQLToken* QueryTokenParseState::expect(PQLTokenType... tokenType) {
  PQLToken* currentToken = tryExpect(tokenType...);
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  return currentToken;
}
