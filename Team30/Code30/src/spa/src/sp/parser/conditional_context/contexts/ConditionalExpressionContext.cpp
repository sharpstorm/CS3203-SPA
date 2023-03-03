#include <memory>
#include "RelationalExpressionContext.h"
#include "ConditionalExpressionContext.h"
#include "common/ast/math/conditional_operand/NotASTNode.h"
#include "common/ast/math/conditional_operand/AndASTNode.h"
#include "common/ast/math/conditional_operand/OrASTNode.h"

using std::make_shared;

ASTNodePtr ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr newNode;
  SourceToken* token = state->getCurrToken();
  if (token == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  int curState = state->getCurrPosition();
  try {
    newNode = processRelationalExpression(state);
    state->setCached(newNode);
    return newNode;
  } catch(const SPError& error) {
    state->restorePosition(curState);
  }

  switch (token->getType()) {
    case SIMPLE_TOKEN_NOT:
      newNode = processNotCondition(state);
      break;
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      newNode = processBiCondition(state);
      break;
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  state->setCached(newNode);
  return newNode;
}

template<class T>
BinaryASTNodePtr ConditionalExpressionContext::
generateConditionalNode(ASTNodePtr leftNode) {
  BinaryASTNodePtr node = make_shared<T>();
  node->setLeftChild(leftNode);
  return node;
}

ASTNodePtr ConditionalExpressionContext::
processNotCondition(SourceParseState* state) {
  // Expect '!' -> '('
  state->expect(SIMPLE_TOKEN_NOT);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Create Condition Expression node and parse condition
  ASTNodePtr leftNode = contextProvider->getContext(COND_CONTEXT)
      ->generateSubtree(state);
  ASTNodePtr newNode = generateConditionalNode<NotASTNode>(leftNode);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

ASTNodePtr
ConditionalExpressionContext::
processBiCondition(SourceParseState* state) {
  // Expect '('
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Parse first condition
  ASTNodePtr leftCondition = contextProvider
      ->getContext(COND_CONTEXT)->generateSubtree(state);

  // Expect ')' -> '&&' / '||' -> '('
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  SourceTokenType type = state->expect(SIMPLE_TOKEN_AND,
                                       SIMPLE_TOKEN_OR)->getType();
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  state->clearCached();

  // Generate Condition node
  BinaryASTNodePtr newNode;
  if (type == SIMPLE_TOKEN_AND) {
    newNode = generateConditionalNode<AndASTNode>(leftCondition);
  } else {
    newNode = generateConditionalNode<OrASTNode>(leftCondition);
  }

  // Parse second condition
  newNode->setRightChild(contextProvider
                             ->getContext(COND_CONTEXT)
                             ->generateSubtree(state));
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

ASTNodePtr ConditionalExpressionContext::
processRelationalExpression(SourceParseState* state) {
  return contextProvider->getContext(REL_CONTEXT)->generateSubtree(state);
}
