#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "PatternTrie.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

class PatternConverter {
 public:
  static ExpressionSequencePtr convertASTToPostfix(
      IAST *tree, const PkbQueryHandler *agent);
  static PatternTriePtr convertASTToTrie(IASTNode *tree, PkbWriter *pkbWriter);

  static SymbolIdent queryVariable(IASTNode *node,
                                   const PkbQueryHandler *agent);
  static SymbolIdent queryConstant(IASTNode *node,
                                   const PkbQueryHandler *agent);

 private:
  static SymbolIdent indexFromNode(IASTNode *node,
                                   const PkbQueryHandler *agent);
  static void traversePostfix(IASTNode *node,
                              ExpressionSequence *output,
                              const PkbQueryHandler *agent);
};
