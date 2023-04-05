#include "RelationalExpressionContext.h"

#include <memory>
#include <utility>

#include "sp/ast/conditional_operand/GtASTNode.h"
#include "sp/ast/conditional_operand/GteASTNode.h"
#include "sp/ast/conditional_operand/LteASTNode.h"
#include "sp/ast/conditional_operand/EqualsASTNode.h"
#include "sp/ast/conditional_operand/NotEqualsASTNode.h"
#include "sp/ast/conditional_operand/LtASTNode.h"

using std::make_unique, std::move;

ASTNodePtr
RelationalExpressionContext::generateSubtree(SourceParseState *state) const {
  ASTNodePtr leftNode = contextProvider
      ->generateSubtree(ConditionalContextType::REL_FACTOR_CONTEXT, state);

  SourceToken *token = state->expect(SIMPLE_TOKEN_GT,
                                     SIMPLE_TOKEN_GTE,
                                     SIMPLE_TOKEN_LT,
                                     SIMPLE_TOKEN_LTE,
                                     SIMPLE_TOKEN_EQUALS,
                                     SIMPLE_TOKEN_NOT_EQUALS);
  BinaryASTNodePtr newNode = generateRelationalNode(token->getType(),
                                                    std::move(leftNode));
  newNode->setRightChild(
      contextProvider
          ->generateSubtree(ConditionalContextType::REL_FACTOR_CONTEXT,
                            state));
  return newNode;
}

BinaryASTNodePtr RelationalExpressionContext::generateRelationalNode(
    SourceTokenType type, ASTNodePtr leftNode) const {
  BinaryASTNodePtr node;
  switch (type) {
    case SIMPLE_TOKEN_GT:
      node = make_unique<GtASTNode>();
      break;
    case SIMPLE_TOKEN_GTE:
      node = make_unique<GteASTNode>();
      break;
    case SIMPLE_TOKEN_LT:
      node = make_unique<LtASTNode>();
      break;
    case SIMPLE_TOKEN_LTE:
      node = make_unique<LteASTNode>();
      break;
    case SIMPLE_TOKEN_EQUALS:
      node = make_unique<EqualsASTNode>();
      break;
    case SIMPLE_TOKEN_NOT_EQUALS:
      node = make_unique<NotEqualsASTNode>();
      break;
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  node->setLeftChild(std::move(leftNode));
  return node;
}
