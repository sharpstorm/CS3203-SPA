#pragma once

#include <memory>

#include "../ExpressionContextType.h"
#include "sp/ast/BinaryASTNode.h"

using std::shared_ptr;

class ExpressionContext: public RecursiveExpressionParseContext {
 public:
  explicit ExpressionContext(IExpressionContextProvider* provider);
  ASTNodePtr generateSubtree(SourceParseState* state) override;

 private:
  BinaryASTNodePtr generateOperand(SourceToken* curToken);
};
