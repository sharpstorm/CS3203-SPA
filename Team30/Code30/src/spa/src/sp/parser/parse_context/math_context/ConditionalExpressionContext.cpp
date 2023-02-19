#include <memory>
#include "RelationalExpressionContext.h"
#include "ConditionalExpressionContext.h"
#include "common/ASTNode/math/conditional_operand/NotASTNode.h"
#include "common/ASTNode/math/conditional_operand/AndASTNode.h"
#include "common/ASTNode/math/conditional_operand/OrASTNode.h"

using std::shared_ptr, std::make_shared;

shared_ptr<ASTNode> ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) {
  shared_ptr<ASTNode> newNode;
  SourceToken* token = state->getCurrToken();
  if (token == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  switch (token->getType()) {
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

template<class T>
shared_ptr<BinaryASTNode>
ConditionalExpressionContext::
generateConditionalNode(shared_ptr<ASTNode> leftNode) {
  shared_ptr<BinaryASTNode> node = make_shared<T>();
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
  shared_ptr<ASTNode> leftNode = contextProvider->getContext(COND_CONTEXT)
      ->generateSubtree(state);
  shared_ptr<ASTNode> newNode = generateConditionalNode<NotASTNode>(leftNode);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processBiCondition(SourceParseState* state) {
  // Expect '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Parse first condition
  shared_ptr<ASTNode> leftCondition = contextProvider
      ->getContext(COND_CONTEXT)->generateSubtree(state);

  // Expect ')' -> '&&' / '||' -> '('
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  SourceTokenType type = expect(state,
                                SIMPLE_TOKEN_AND, SIMPLE_TOKEN_OR)->getType();
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Generate Condition node
  shared_ptr<BinaryASTNode> newNode;
  if (type == SIMPLE_TOKEN_AND) {
    newNode = generateConditionalNode<AndASTNode>(leftCondition);
  } else {
    newNode = generateConditionalNode<OrASTNode>(leftCondition);
  }

  // Parse second condition
  newNode->setRightChild(contextProvider->getContext(COND_CONTEXT)
                             ->generateSubtree(state));
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

shared_ptr<ASTNode>
ConditionalExpressionContext::
processRelationalExpression(SourceParseState* state) {
  return contextProvider->getContext(REL_CONTEXT)->generateSubtree(state);
}
