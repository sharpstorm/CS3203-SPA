#pragma once
#include <vector>
#include "sp/common/SourceToken.h"
#include "common/ast/ASTNode.h"
#include "sp/errors/SPError.h"

using std::vector;

class SourceParseState {
 public:
  explicit SourceParseState(vector<SourceToken>* tokens);
  void advanceToken();
  SourceToken* getCurrToken();
  bool currTokenIsOfType(SourceTokenType type);
  SourceToken* peekNextToken();
  bool nextTokenIsOfType(SourceTokenType type);
  bool isEnd();
  int getCurrPosition();
  void restorePosition(int pos);

  void setCached(ASTNodePtr node);
  void clearCached();
  bool hasCached();
  ASTNodePtr getCached();

  int getLineNumber();
  void advanceLine();

  template<typename... SourceTokenType>
  SourceToken *expect(SourceTokenType... tokenType);
  SourceToken* expectVarchar();

 private:
  int curIndex;
  int tokenLength;
  int lineNumber;
  vector<SourceToken>* tokens;
  ASTNodePtr curCache;
};

template<typename... SourceTokenType>
SourceToken* SourceParseState::expect(SourceTokenType... tokenType) {
  SourceToken* currentToken = getCurrToken();

  if (currentToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}
