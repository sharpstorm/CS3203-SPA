#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "PatternTrie.h"

class PatternConverter {
 public:
  static ExpressionSequencePtr convertASTToPostfix(IAST* tree);
  static PatternTriePtr convertASTToTrie(IASTNode* tree);

 private:
  static void traversePostfix(IASTNode *node, ExpressionSequence *output);
};
