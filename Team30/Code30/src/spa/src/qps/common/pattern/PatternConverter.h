#pragma once

#include "common/pattern/ExpressionSequence.h"
#include "common/ast/IAST.h"
#include "common/pattern/PatternTrie.h"

#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

class PatternConverter {
 public:
  static ExpressionSequencePtr convertASTToPostfix(
      IAST *tree, const PkbQueryHandler *agent);
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
