#pragma once

#include <memory>
#include "common/ast/IAST.h"
#include "ExpressionSequence.h"

using std::unique_ptr;

class PatternTrie {
 public:
  explicit PatternTrie(IASTPtr ast);
  bool isMatchFull(ExpressionSequence* sequence);
  bool isMatchPartial(ExpressionSequence* sequence);

 private:
  IASTPtr ast;

  bool traverseForMatch(IASTNodePtr curNode, ExpressionSequence* sequence);
  bool isMatchNode(IASTNodePtr node, ExpressionSequence* sequence);
};

typedef unique_ptr<PatternTrie> PatternTriePtr;
