#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "PatternTrie.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

class PatternConverter {
 public:
  static ExpressionSequencePtr convertASTToPostfix(IAST* tree,
                                                   PkbQueryHandler* handler);
  static PatternTriePtr convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter);

 private:
  static uint16_t indexFromNode(IASTNode* node, PkbQueryHandler* handler);
  static void traversePostfix(IASTNode *node,
                              ExpressionSequence *output,
                              PkbQueryHandler* handler);
};
