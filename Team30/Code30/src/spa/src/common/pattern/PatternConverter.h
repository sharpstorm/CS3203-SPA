#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "PatternTrie.h"
#include "pkb/writers/PkbWriter.h"
#include "qps/executor/QueryExecutorAgent.h"

class PatternConverter {
 public:
  static ExpressionSequencePtr convertASTToPostfix(IAST* tree,
                                                   QueryExecutorAgent agent);
  static PatternTriePtr convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter);

 private:
  static uint16_t indexFromNode(IASTNode* node, QueryExecutorAgent agent);
  static void traversePostfix(IASTNode *node,
                              ExpressionSequence *output,
                              QueryExecutorAgent agent);
};
