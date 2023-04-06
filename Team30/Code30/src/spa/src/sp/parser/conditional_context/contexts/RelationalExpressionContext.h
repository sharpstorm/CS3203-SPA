#pragma once

#include <memory>

#include "sp/ast/BinaryASTNode.h"
#include "../ConditionalContextType.h"

class RelationalExpressionContext: public RecursiveConditionalParseContext {
 public:
  explicit RelationalExpressionContext(IConditionalContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state) const override;

 private:
  BinaryASTNodePtr
  generateRelationalNode(const SourceToken *token, ASTNodePtr leftNode) const;
};
