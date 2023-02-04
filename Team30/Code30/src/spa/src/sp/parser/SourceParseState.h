#pragma once
#include <vector>
#include <memory>
#include "sp/common/SourceToken.h"
#include "sp/common/ASTNode/ASTNode.h"

using std::shared_ptr;

class SourceParseState {
 public:
  SourceParseState(vector<SourceToken>* tokens);
  void advanceToken();
  SourceToken* getCurrToken();
  SourceToken* peekNextToken();
  bool nextTokenIsOfType(SourceTokenType type);
  bool isEnd();
  shared_ptr<ASTNode> curRoot;

 private:
  vector<SourceToken>* tokens;
  int curIndex;
  int tokenLength;
};