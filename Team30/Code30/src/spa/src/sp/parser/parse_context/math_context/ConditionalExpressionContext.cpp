#include <memory>
#include "RelationalExpressionContext.h"
#include "ConditionalExpressionContext.h"
#include "common/ASTNode/math/conditional_operand/NotASTNode.h"
#include "common/ASTNode/math/conditional_operand/AndASTNode.h"
#include "common/ASTNode/math/conditional_operand/OrASTNode.h"

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

shared_ptr<BinaryASTNode>
ConditionalExpressionContext::
generateConditionalNode(shared_ptr<ASTNode> leftNode, SourceTokenType type) {
  shared_ptr<BinaryASTNode> node;
  if (type == SIMPLE_TOKEN_AND) {
    node = shared_ptr<BinaryASTNode>(new AndASTNode());
  } else if (type == SIMPLE_TOKEN_OR) {
    node = shared_ptr<BinaryASTNode>(new OrASTNode());
  } else {
    node = shared_ptr<BinaryASTNode>(new NotASTNode());
  }
  node->setLeftChild(leftNode);
  return node;
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
          generateSubtree(state), SIMPLE_TOKEN_NOT);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processBiCondition(SourceParseState* state) {
  // Expect '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Expect ')' -> '&&' / '||' -> '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  SourceTokenType type = expect(state,
                                SIMPLE_TOKEN_AND, SIMPLE_TOKEN_OR)->getType();
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Generate Condition node and parse first condition
  shared_ptr<BinaryASTNode> newNode =
      generateConditionalNode(contextProvider->
          getContext(COND_CONTEXT)->
          generateSubtree(state), type);

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
