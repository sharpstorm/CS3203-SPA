#pragma once
#include <vector>
#include "sp/common/SourceToken.h"
#include "sp/ast/ASTNode.h"
#include "sp/errors/SPError.h"

using std::vector;

typedef int LineNumber;

class SourceParseState {
 public:
  explicit SourceParseState(vector<SourceToken>* tokens);
  void advanceToken();
  SourceToken* getCurrToken();
  SourceToken* peekNextToken();
  bool nextTokenIsOfType(SourceTokenType type);

  bool isEnd();
  int getCurrPosition();
  void restorePosition(int pos);

  void cacheNode(ASTNodePtr node);
  bool hasCached();
  ASTNodePtr consumeCache();

  LineNumber getLineNumber();

  template<typename... SourceTokenType>
  bool currTokenIsOfType(SourceTokenType... type);

  template<typename... SourceTokenType>
  SourceToken *expect(SourceTokenType... tokenType);

  template<typename... SourceTokenType>
  SourceToken *tryExpect(SourceTokenType... tokenType);

  SourceToken* expectVarchar();

 private:
  int curIndex;
  int tokenLength;
  LineNumber lineNumber;
  vector<SourceToken>* tokens;
  ASTNodePtr curCache;
};

template<typename... SourceTokenType>
SourceToken* SourceParseState::expect(SourceTokenType... tokenType) {
  if (currTokenIsOfType(tokenType...)) {
    SourceToken* currentToken = getCurrToken();
    advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}

template<typename... SourceTokenType>
SourceToken* SourceParseState::tryExpect(SourceTokenType... tokenType) {
  if (currTokenIsOfType(tokenType...)) {
    SourceToken* currentToken = getCurrToken();
    advanceToken();
    return currentToken;
  }
  return nullptr;
}

template<typename... SourceTokenType>
bool SourceParseState::currTokenIsOfType(SourceTokenType... type) {
  SourceToken* token = getCurrToken();
  if (token == nullptr) {
    return false;
  }

  return token->isType(type...);
}
