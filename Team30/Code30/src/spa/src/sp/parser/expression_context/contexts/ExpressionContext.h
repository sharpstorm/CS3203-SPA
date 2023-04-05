#pragma once

#include "../ExpressionContextType.h"
#include "sp/ast/BinaryASTNode.h"

class ExpressionContext : public RecursiveExpressionParseContext {
 public:
  explicit ExpressionContext(IExpressionContextProvider *provider);
  ASTNodePtr generateSubtree(SourceParseState *state) const override;

 private:
  BinaryASTNodePtr generateOperand(SourceParseState *state) const;
  BinaryASTNodePtr makeNode(const SourceToken *token) const;
};
