#include "SourceParseState.h"
#include <stack>
#include <iostream>

SourceParseState::SourceParseState(vector<SourceToken>* t) {
  curIndex = 0;
  tokenLength = t->size();
  tokens = t;
  curRoot = nullptr;
}

void SourceParseState::advanceToken() {
  if (!isEnd()) {
    curIndex++;
  }
}

bool SourceParseState::isEnd() {
  return curIndex >= tokenLength;
}

SourceToken *SourceParseState::getCurrToken() {
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
