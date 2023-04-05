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

SourceToken *SourceParseState::getCurrToken() const {
  if (curIndex >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex);
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
  SourceToken *currentToken = getCurrToken();

  if (currentToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if (currentToken->isVarchar()) {
    advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}
