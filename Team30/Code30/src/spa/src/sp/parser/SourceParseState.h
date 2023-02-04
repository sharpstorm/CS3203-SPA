#pragma once
#include <vector>
#include <memory>
#include "sp/common/SourceToken.h"
#include "sp/common/ASTNode/ASTNode.h"

using std::shared_ptr;

enum SourceParseLevel {
  SOURCE_PARSE_LEVEL_NONE,
  SOURCE_PARSE_LEVEL_EXPR = 0x1,
  SOURCE_PARSE_LEVEL_TERM = 0x3,
  SOURCE_PARSE_LEVEL_FACTOR = 0x7,
};

const int IS_EXPR_MASK = 0x1;
const int IS_TERM_MASK = 0x2;
const int IS_FACTOR_MASK = 0x4;

class SourceParseState {
 public:
  SourceParseState(vector<SourceToken>* tokens);
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

 private:
  vector<SourceToken>* tokens;
  int curIndex;
  int tokenLength;
  shared_ptr<ASTNode> curCache;
};