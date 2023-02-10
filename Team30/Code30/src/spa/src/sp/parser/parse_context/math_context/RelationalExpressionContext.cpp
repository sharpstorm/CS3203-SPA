#include <memory>
#include "RelationalExpressionContext.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"

using std::shared_ptr;

shared_ptr<ASTNode>
RelationalExpressionContext::generateSubtree(SourceParseState *state) {
  shared_ptr<ASTNode> leftNode = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);

  SourceToken* token = expect(state,
                              SIMPLE_TOKEN_GT,
                              SIMPLE_TOKEN_GTE,
                              SIMPLE_TOKEN_LT,
                              SIMPLE_TOKEN_LTE,
                              SIMPLE_TOKEN_EQUALS,
                              SIMPLE_TOKEN_NOT_EQUALS);
  state->clearCached();
  shared_ptr<RelationalExpressionASTNode> newNode =
      generateRelationalNode(state, leftNode);
  newNode->setRightChild(contextProvider->
      getContext(REL_FACTOR_CONTEXT)->generateSubtree(state));
  state->setCached(newNode);
  return newNode;
}

shared_ptr<RelationalExpressionASTNode>
RelationalExpressionContext::generateRelationalNode
    (SourceParseState* state, shared_ptr<ASTNode> leftNode) {
  RelationalExpressionASTNode* node = new RelationalExpressionASTNode();
  shared_ptr<RelationalExpressionASTNode> newNode =
      shared_ptr<RelationalExpressionASTNode>(node);
  node->setLeftChild(leftNode);
  return newNode;
}
