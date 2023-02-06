#include <memory>
#include "RelationalExpressionContext.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"
#include "ConditionalExpressionContext.h"
#include "sp/common/ASTNode/math/ConditionalExpressionASTNode.h"

using std::shared_ptr;

bool ConditionalExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) {
  shared_ptr<ASTNode> leftNode;
  shared_ptr<ConditionalExpressionASTNode> newNode;
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_NOT:
      expect(state, SIMPLE_TOKEN_NOT);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      leftNode = contextProvider->getContext(COND_CONTEXT)->
          generateSubtree(state);
      state->clearCached();
      newNode = generateConditionalNode(state, leftNode);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      break;
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      leftNode = contextProvider->getContext(COND_CONTEXT)->
          generateSubtree(state);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      newNode = generateConditionalNode(state, leftNode);
      state->clearCached();
      expect(state, SIMPLE_TOKEN_AND, SIMPLE_TOKEN_OR);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      newNode->setRightChild(contextProvider->getContext(COND_CONTEXT)->
      generateSubtree(state));
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      break;
    default:
      leftNode = contextProvider->getContext(REL_CONTEXT)->
          generateSubtree(state);
      state->setCached(leftNode);
      return leftNode;
  }
  state->setCached(newNode);
  return newNode;
}

shared_ptr<ConditionalExpressionASTNode>
    ConditionalExpressionContext::generateConditionalNode(
        SourceParseState* state, shared_ptr<ASTNode> leftNode) {
  ConditionalExpressionASTNode* node = new ConditionalExpressionASTNode();
  shared_ptr<ConditionalExpressionASTNode> newNode =
      shared_ptr<ConditionalExpressionASTNode>(node);
  node->setLeftChild(leftNode);
  return newNode;
}
