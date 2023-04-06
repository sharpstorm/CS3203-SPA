#include <memory>
#include <utility>

#include "RelationalExpressionContext.h"
#include "ConditionalExpressionContext.h"
#include "sp/ast/conditional_operand/NotASTNode.h"
#include "sp/ast/conditional_operand/AndASTNode.h"
#include "sp/ast/conditional_operand/OrASTNode.h"

using std::make_unique;

ASTNodePtr ConditionalExpressionContext::generateSubtree(
    SourceParseState *state) const {
  ASTNodePtr newNode;
  if (state->isEnd()) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  auto stateSnapshot = state->savePosition();
  try {
    return processRelationalExpression(state);
  } catch (const SPError &error) {
    state->restorePosition(stateSnapshot);
  }

  switch (state->getCurrTokenType()) {
    case SIMPLE_TOKEN_NOT:newNode = processNotCondition(state);
      break;
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:newNode = processBiCondition(state);
      break;
    default:throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  return newNode;
}

template<class T>
BinaryASTNodePtr ConditionalExpressionContext::
generateConditionalNode(ASTNodePtr leftNode) {
  BinaryASTNodePtr node = make_unique<T>();
  node->setLeftChild(std::move(leftNode));
  return node;
}

ASTNodePtr ConditionalExpressionContext::
processNotCondition(SourceParseState *state) const {
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
processBiCondition(SourceParseState *state) const {
  // Expect '('
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Parse first condition
  ASTNodePtr leftCondition = contextProvider
      ->generateSubtree(ConditionalContextType::COND_CONTEXT, state);

  // Expect ')' -> '&&' / '||' -> '('
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  SourceToken *token = state->expect(SIMPLE_TOKEN_AND,
                                     SIMPLE_TOKEN_OR);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);

  // Generate Condition node
  BinaryASTNodePtr newNode = makeNode(token, std::move(leftCondition));

  // Parse second condition
  ASTNodePtr rightChild = contextProvider
      ->generateSubtree(ConditionalContextType::COND_CONTEXT, state);
  newNode->setRightChild(std::move(rightChild));
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  return newNode;
}

ASTNodePtr ConditionalExpressionContext::
processRelationalExpression(SourceParseState *state) const {
  return contextProvider
      ->generateSubtree(ConditionalContextType::REL_CONTEXT, state);
}

BinaryASTNodePtr ConditionalExpressionContext::makeNode(
    const SourceToken *token, ASTNodePtr leftCondition) const {
  if (token->getType() == SIMPLE_TOKEN_AND) {
    return generateConditionalNode<AndASTNode>(std::move(leftCondition));
  } else {
    return generateConditionalNode<OrASTNode>(std::move(leftCondition));
  }
}
