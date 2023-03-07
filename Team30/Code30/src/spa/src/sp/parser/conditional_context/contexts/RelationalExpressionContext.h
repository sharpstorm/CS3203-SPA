#pragma once

#include <memory>

#include "sp/ast/BinaryASTNode.h"
#include "../ConditionalContextType.h"

class RelationalExpressionContext: public RecursiveConditionalParseContext {
 public:
  explicit RelationalExpressionContext(IConditionalContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state) override;

 private:
  BinaryASTNodePtr
  generateRelationalNode(SourceTokenType type, ASTNodePtr leftNode);
};
