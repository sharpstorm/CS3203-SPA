#pragma once

#include <memory>

#include "common/ast/BinaryASTNode.h"
#include "../ConditionalContextType.h"

class RelationalExpressionContext: public RecursiveConditionalParseContext {
 public:
  explicit RelationalExpressionContext(IConditionalContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  BinaryASTNodePtr
  generateRelationalNode(SourceTokenType type, ASTNodePtr leftNode);
};
