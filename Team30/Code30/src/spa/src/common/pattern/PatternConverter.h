#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "PatternTrie.h"
#include "pkb/writers/PkbWriter.h"

class PatternConverter {
 public:
  // TODO Take in PKB Querier
  static ExpressionSequencePtr convertASTToPostfix(IAST* tree);
  // TODO Take in PKB Writer
  static PatternTriePtr convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter);

 private:
  // TODO Take in PKB Querier
  static void traversePostfix(IASTNode *node, ExpressionSequence *output);
};
