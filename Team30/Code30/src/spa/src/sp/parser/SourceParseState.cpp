#include "SourceParseState.h"

#include <utility>

SourceParseState::SourceParseState(SourceTokenStream *t) :
    curIndex(0),
    lineNumber(0),
    tokenLength(t->size()),
    tokens(t),
    curCache(nullptr) {
}

void SourceParseState::advanceToken() {
  if (!isEnd()) {
    curIndex++;
  }
}

bool SourceParseState::isEnd() const {
  return curIndex >= tokenLength;
}

SourceParseState::SourceParseStateSnapshot SourceParseState::savePosition() {
  return SourceParseStateSnapshot(curIndex);
}

void SourceParseState::restorePosition(const SourceParseStateSnapshot &pos) {
  curIndex = pos.lineNo;
}

SourceTokenType SourceParseState::getCurrTokenType() const {
  if (isEnd()) {
    return SIMPLE_TOKEN_NULL;
  }
  return getCurrToken()->getType();
}

bool SourceParseState::isCurrTokenVarchar() const {
  if (isEnd()) {
    return false;
  }
  return getCurrToken()->isVarchar();
}

SourceToken *SourceParseState::peekNextToken() const {
  if (curIndex + 1 >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex + 1);
}

bool SourceParseState::nextTokenIsOfType(SourceTokenType type) const {
  SourceToken *token = peekNextToken();
  if (token == nullptr) {
    return false;
  }
  return token->isType(type);
}

void SourceParseState::cacheNode(ASTNodePtr node) {
  curCache = std::move(node);
}

bool SourceParseState::hasCached() const {
  return curCache != nullptr;
}

ASTNodePtr SourceParseState::consumeCache() {
  ASTNodePtr temp = std::move(curCache);
  curCache = nullptr;
  return temp;
}

LineNumber SourceParseState::getLineNumber() const {
  return lineNumber;
}

void SourceParseState::advanceLine() {
  lineNumber += 1;
}

SourceToken *SourceParseState::expectVarchar() {
  if (isEnd()) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  SourceToken *currentToken = getCurrToken();
  if (currentToken->isVarchar()) {
    advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}

SourceToken *SourceParseState::getCurrToken() const {
  return &tokens->at(curIndex);
}
