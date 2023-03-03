#pragma once

#include <memory>

#include "../ExpressionContextType.h"
#include "common/ast/BinaryASTNode.h"

using std::shared_ptr;

class ExpressionContext: public RecursiveExpressionParseContext {
 public:
  explicit ExpressionContext(IExpressionContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  BinaryASTNodePtr generateOperand(SourceToken* curToken);
};
