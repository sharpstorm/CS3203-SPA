#include <memory>
#include "RelationalExpressionContext.h"
#include "common/ASTNode/math/RelationalExpressionASTNode.h"
#include "ConditionalExpressionContext.h"
#include "common/ASTNode/math/ConditionalExpressionASTNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) {
  shared_ptr<ASTNode> newNode;
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_NOT:
      newNode = processNotCondition(state);
      break;
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      newNode = processBiCondition(state);
      break;
    default:
      newNode = processRelationalExpression(state);
      break;
  }
  state->setCached(newNode);
  return newNode;
}

shared_ptr<ConditionalExpressionASTNode>
ConditionalExpressionContext::
generateConditionalNode(shared_ptr<ASTNode> leftNode) {
  ConditionalExpressionASTNode* node = new ConditionalExpressionASTNode();
  shared_ptr<ConditionalExpressionASTNode> newNode =
      shared_ptr<ConditionalExpressionASTNode>(node);
  node->setLeftChild(leftNode);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processNotCondition(SourceParseState* state) {
  // Expect '!' -> '('
  expect(state, SIMPLE_TOKEN_NOT);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Create Condition Expression node and parse condition
  shared_ptr<ASTNode> newNode =
      generateConditionalNode(contextProvider->
          getContext(COND_CONTEXT)->
          generateSubtree(state));
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processBiCondition(SourceParseState* state) {
  // Expect '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Generate Condition node and parse first condition
  shared_ptr<ConditionalExpressionASTNode> newNode =
      generateConditionalNode(contextProvider->
          getContext(COND_CONTEXT)->
          generateSubtree(state));

  // Expect ')' -> '&&' / '||' -> '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  expect(state, SIMPLE_TOKEN_AND, SIMPLE_TOKEN_OR);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Parse second condition
  newNode->setRightChild(contextProvider->
      getContext(COND_CONTEXT)->
      generateSubtree(state));
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processRelationalExpression(SourceParseState* state) {
  shared_ptr<ASTNode> newNode = contextProvider->
      getContext(REL_CONTEXT)->
      generateSubtree(state);
  return newNode;
}