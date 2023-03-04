#pragma once

#include <memory>
#include "common/ast/IAST.h"
#include "ExpressionSequence.h"

using std::unique_ptr, std::shared_ptr;

class PatternTrie {
 public:
  explicit PatternTrie(IASTNodePtr ast);
  bool isMatchFull(ExpressionSequence* sequence);
  bool isMatchPartial(ExpressionSequence* sequence);

 private:
  IASTNodePtr root;

  bool traverseForMatch(IASTNodePtr curNode, ExpressionSequence* sequence);
  bool isMatchNode(IASTNodePtr node, ExpressionSequence* sequence);
};

typedef unique_ptr<PatternTrie> PatternTriePtr;
typedef shared_ptr<PatternTrie> PatternTrieSPtr;
