#pragma once

#include "sp/ast/BinaryASTNode.h"

class ConditionalExpressionContext: public RecursiveConditionalParseContext {
 public:
  explicit ConditionalExpressionContext(IConditionalContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  template<class T>
  static BinaryASTNodePtr generateConditionalNode(ASTNodePtr leftNode);

  ASTNodePtr processNotCondition(SourceParseState* state);
  ASTNodePtr processBiCondition(SourceParseState* state);
  ASTNodePtr processRelationalExpression(SourceParseState* state);
};
