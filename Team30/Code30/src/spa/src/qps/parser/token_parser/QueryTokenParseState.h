#pragma once

#include "token_stream/QueryTokenStream.h"
#include "../../errors/QPSParserSyntaxError.h"
#include "qps/common/synonym/PQLTypes.h"

class QueryTokenParseState {
 private:
  QueryTokenStream tokenStream;
  static void assertNotNull(const PQLToken *token);

 public:
  explicit QueryTokenParseState(PQLTokenList *tokens);
  bool isTokenStreamEnd() const;
  void advanceToken();

  PQLTokenType getCurrentTokenType() const;
  bool isCurrentTokenType(PQLTokenType type) const;
  bool isCurrentTokenCategory(PQLTokenCategory category) const;

  template<typename... T>
  const PQLToken *expect(T... tokenType);
  template<typename... T>
  const PQLToken *tryExpect(T... tokenType);
  PQLSynonymName expectSynName();
};

template<typename... PQLTokenType>
const PQLToken *QueryTokenParseState::tryExpect(PQLTokenType... tokenType) {
  const PQLToken *currentToken = tokenStream.getCurrentToken();
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
const PQLToken *QueryTokenParseState::expect(PQLTokenType... tokenType) {
  const PQLToken *currentToken = tryExpect(tokenType...);
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  return currentToken;
}
