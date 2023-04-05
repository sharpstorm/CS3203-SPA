#pragma once

#include "../ExpressionContextType.h"
#include "sp/ast/BinaryASTNode.h"

using std::shared_ptr;

class TermContext : public RecursiveExpressionParseContext {
 public:
  explicit TermContext(IExpressionContextProvider *provider) :
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;

 private:
  BinaryASTNodePtr generateOperand(SourceParseState *state) const;
  BinaryASTNodePtr makeNode(const SourceToken *token) const;
};
