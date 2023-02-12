#pragma once
#include <vector>
#include <memory>
#include "sp/common/SourceToken.h"
#include "common/ASTNode/ASTNode.h"

using std::vector, std::shared_ptr;

class SourceParseState {
 public:
  explicit SourceParseState(vector<SourceToken>* tokens);
  void advanceToken();
  SourceToken* getCurrToken();
  SourceToken* peekNextToken();
  bool nextTokenIsOfType(SourceTokenType type);
  bool isEnd();
  bool isAtLast();

  void setCached(shared_ptr<ASTNode> node);
  void clearCached();
  bool hasCached();
  shared_ptr<ASTNode> getCached();

  int getLineNumber();
  void advanceLine();

 private:
  int curIndex;
  int tokenLength;
  int lineNumber;
  vector<SourceToken>* tokens;
  shared_ptr<ASTNode> curCache;
};
