#pragma once

#include "common/ast/math/math_operand/TimesASTNode.h"
#include "../ExpressionContextType.h"

using std::shared_ptr;

class TermContext: public RecursiveExpressionParseContext {
 public:
  explicit TermContext(IExpressionContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  BinaryASTNodePtr generateOperand(SourceToken* curToken);
};
