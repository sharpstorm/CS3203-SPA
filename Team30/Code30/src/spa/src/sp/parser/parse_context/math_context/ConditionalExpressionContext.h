#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ASTNode/math/RelationalExpressionASTNode.h"
#include "common/ASTNode/math/ConditionalExpressionASTNode.h"

using std::shared_ptr;

class ConditionalExpressionContext: public RecursiveParseContext {
 public:
  explicit ConditionalExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<ConditionalExpressionASTNode>
  generateConditionalNode(shared_ptr<ASTNode> leftNode);
  shared_ptr<ASTNode>
  processNotCondition(SourceParseState* state);
  shared_ptr<ASTNode>
  processBiCondition(SourceParseState* state);
  shared_ptr<ASTNode>
  processRelationalExpression(SourceParseState* state);
};
