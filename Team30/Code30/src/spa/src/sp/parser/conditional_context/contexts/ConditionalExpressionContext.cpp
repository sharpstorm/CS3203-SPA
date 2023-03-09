#include <memory>
#include <utility>

#include "RelationalExpressionContext.h"
#include "ConditionalExpressionContext.h"
#include "sp/ast/conditional_operand/NotASTNode.h"
#include "sp/ast/conditional_operand/AndASTNode.h"
#include "sp/ast/conditional_operand/OrASTNode.h"

using std::make_unique;

ASTNodePtr ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr newNode;
  SourceToken* token = state->getCurrToken();
  if (token == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  int curState = state->getCurrPosition();
  try {
    return processRelationalExpression(state);
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
  return newNode;
}

template<class T>
BinaryASTNodePtr ConditionalExpressionContext::
generateConditionalNode(ASTNodePtr leftNode) {
  BinaryASTNodePtr node = make_unique<T>();
  node->setLeftChild(std::move(leftNode));
  return std::move(node);
}

ASTNodePtr ConditionalExpressionContext::
processNotCondition(SourceParseState* state) {
  // Expect '!' -> '('
  state->expect(SIMPLE_TOKEN_NOT);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Create Condition Expression node and parse condition
  ASTNodePtr leftNode = contextProvider
      ->generateSubtree(ConditionalContextType::COND_CONTEXT, state);
  ASTNodePtr newNode = generateConditionalNode<NotASTNode>(std::move(leftNode));
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
      ->generateSubtree(ConditionalContextType::COND_CONTEXT, state);

  // Expect ')' -> '&&' / '||' -> '('
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  SourceTokenType type = state->expect(SIMPLE_TOKEN_AND,
                                       SIMPLE_TOKEN_OR)->getType();
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Generate Condition node
  BinaryASTNodePtr newNode;
  if (type == SIMPLE_TOKEN_AND) {
    newNode = generateConditionalNode<AndASTNode>(std::move(leftCondition));
  } else {
    newNode = generateConditionalNode<OrASTNode>(std::move(leftCondition));
  }

  // Parse second condition
  newNode->setRightChild(
      contextProvider
          ->generateSubtree(ConditionalContextType::COND_CONTEXT, state));
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return std::move(newNode);
}

ASTNodePtr ConditionalExpressionContext::
processRelationalExpression(SourceParseState* state) {
  return contextProvider
      ->generateSubtree(ConditionalContextType::REL_CONTEXT, state);
}
