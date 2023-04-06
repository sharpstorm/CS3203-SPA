#pragma once

#include "sp/ast/BinaryASTNode.h"

class ConditionalExpressionContext : public RecursiveConditionalParseContext {
 public:
  explicit ConditionalExpressionContext(IConditionalContextProvider *provider) :
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;

 private:
  template<class T>
  static BinaryASTNodePtr generateConditionalNode(ASTNodePtr leftNode);

  ASTNodePtr processNotCondition(SourceParseState *state) const;
  ASTNodePtr processBiCondition(SourceParseState *state) const;
  ASTNodePtr processRelationalExpression(SourceParseState *state) const;

  BinaryASTNodePtr makeNode(const SourceToken *token,
                            ASTNodePtr leftCondition) const;
};
