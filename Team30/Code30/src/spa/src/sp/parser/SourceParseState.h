#pragma once
#include <vector>
#include "sp/common/SourceToken.h"
#include "common/ast/ASTNode.h"

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

 private:
  int curIndex;
  int tokenLength;
  int lineNumber;
  vector<SourceToken>* tokens;
  ASTNodePtr curCache;
};
