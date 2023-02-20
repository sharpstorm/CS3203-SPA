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

int SourceParseState::getCurrPosition() {
  return curIndex;
}

void SourceParseState::restorePosition(int pos) {
  curIndex = pos;
}

SourceToken *SourceParseState::getCurrToken() {
  if (curIndex >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex);
}

bool SourceParseState::currTokenIsOfType(SourceTokenType type) {
  SourceToken* token = getCurrToken();
  if (token == nullptr) {
    return false;
  }
  return token->isType(type);
}

SourceToken *SourceParseState::peekNextToken() {
  if (curIndex + 1 >= tokenLength) {
    return nullptr;
  }
  return &tokens->at(curIndex + 1);
}

bool SourceParseState::nextTokenIsOfType(SourceTokenType type) {
  SourceToken* token = peekNextToken();
  if (token == nullptr) {
    return false;
  }
  return token->isType(type);
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
