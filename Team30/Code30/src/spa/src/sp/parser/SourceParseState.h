#pragma once

#include "sp/common/SourceToken.h"
#include "sp/ast/ASTNode.h"
#include "sp/errors/SPError.h"
#include "../SPTypes.h"

class SourceParseState {
  typedef int SPParserCursor;

 public:
  class SourceParseStateSnapshot {
   private:
    LineNumber lineNo;
    friend SourceParseState;

   public:
    explicit SourceParseStateSnapshot(const LineNumber &lineNo) :
        lineNo(lineNo) {}
  };

  explicit SourceParseState(SourceTokenStream *tokens);

  SourceTokenType getCurrTokenType() const;
  bool isCurrTokenVarchar() const;
  template<typename... SourceTokenType>
  bool currTokenIsOfType(SourceTokenType... type) const;
  bool nextTokenIsOfType(SourceTokenType type) const;

  bool isEnd() const;
  SourceParseStateSnapshot savePosition();
  void restorePosition(const SourceParseStateSnapshot &snapshot);

  void cacheNode(ASTNodePtr node);
  bool hasCached() const;
  ASTNodePtr consumeCache();

  LineNumber getLineNumber() const;
  void advanceLine();

  template<typename... SourceTokenType>
  SourceToken *expect(SourceTokenType... tokenType);

  template<typename... TokenType>
  SourceTokenType expectType(TokenType... tokenType);

  template<typename... SourceTokenType>
  SourceToken *tryExpect(SourceTokenType... tokenType);
  SourceToken *expectVarchar();

 private:
  SPParserCursor curIndex;
  int tokenLength;
  LineNumber lineNumber;
  SourceTokenStream *tokens;
  ASTNodePtr curCache;

  SourceToken *peekNextToken() const;
  SourceToken *getCurrToken() const;
  void advanceToken();
};

template<typename... SourceTokenType>
SourceToken *SourceParseState::expect(SourceTokenType... tokenType) {
  SourceToken *currentToken = tryExpect(tokenType...);
  if (currentToken == nullptr) {
    throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  return currentToken;
}

template<typename... TokenType>
SourceTokenType SourceParseState::expectType(TokenType... tokenType) {
  SourceToken *currentToken = tryExpect(tokenType...);
  if (currentToken == nullptr) {
    throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  return currentToken->getType();
}

template<typename... SourceTokenType>
SourceToken *SourceParseState::tryExpect(SourceTokenType... tokenType) {
  if (currTokenIsOfType(tokenType...)) {
    SourceToken *currentToken = getCurrToken();
    advanceToken();
    return currentToken;
  }
  return nullptr;
}

template<typename... SourceTokenType>
bool SourceParseState::currTokenIsOfType(SourceTokenType... type) const {
  if (isEnd()) {
    return false;
  }

  return getCurrToken()->isType(type...);
}
