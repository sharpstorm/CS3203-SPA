#include "SourceParseState.h"

SourceParseState::SourceParseState(vector<SourceToken>* t):
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

bool SourceParseState::isEnd() {
  return curIndex >= tokenLength;
}

bool SourceParseState::isAtLast() {
  return curIndex == tokenLength - 1;
}

SourceToken *SourceParseState::getCurrToken() {
  if (curIndex >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex);
}

SourceToken *SourceParseState::peekNextToken() {
  if (curIndex + 1 >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex + 1);
}

bool SourceParseState::nextTokenIsOfType(SourceTokenType type) {
  return peekNextToken()->isType(type);
}

void SourceParseState::setCached(ASTNodePtr node) {
  curCache = node;
}

void SourceParseState::clearCached() {
  curCache = nullptr;
}

bool SourceParseState::hasCached() {
  return curCache != nullptr;
}

ASTNodePtr SourceParseState::getCached() {
  return curCache;
}

int SourceParseState::getLineNumber() {
  return lineNumber;
}

void SourceParseState::advanceLine() {
  lineNumber += 1;
}
