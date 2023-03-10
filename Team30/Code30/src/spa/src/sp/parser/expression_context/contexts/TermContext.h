#pragma once

#include "../ExpressionContextType.h"
#include "sp/ast/BinaryASTNode.h"

using std::shared_ptr;

class TermContext: public RecursiveExpressionParseContext {
 public:
  explicit TermContext(IExpressionContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state) override;

 private:
  BinaryASTNodePtr generateOperand(SourceParseState *state);
};
