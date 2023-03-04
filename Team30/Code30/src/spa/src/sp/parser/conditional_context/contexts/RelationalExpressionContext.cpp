#include <memory>
#include "RelationalExpressionContext.h"

#include "sp/ast/conditional_operand/GtASTNode.h"
#include "sp/ast/conditional_operand/GteASTNode.h"
#include "sp/ast/conditional_operand/LteASTNode.h"
#include "sp/ast/conditional_operand/EqualsASTNode.h"
#include "sp/ast/conditional_operand/NotEqualsASTNode.h"
#include "sp/ast/conditional_operand/LtASTNode.h"

using std::make_shared;

ASTNodePtr
RelationalExpressionContext::generateSubtree(SourceParseState *state) {
  state->clearCached();
  ASTNodePtr leftNode = contextProvider
      ->getContext(REL_FACTOR_CONTEXT)
      ->generateSubtree(state);

  SourceToken* token = state->expect(SIMPLE_TOKEN_GT,
                                     SIMPLE_TOKEN_GTE,
                                     SIMPLE_TOKEN_LT,
                                     SIMPLE_TOKEN_LTE,
                                     SIMPLE_TOKEN_EQUALS,
                                     SIMPLE_TOKEN_NOT_EQUALS);
  state->clearCached();
  BinaryASTNodePtr newNode =
      generateRelationalNode(token->getType(), leftNode);
  newNode->setRightChild(contextProvider->
      getContext(REL_FACTOR_CONTEXT)->generateSubtree(state));
  state->setCached(newNode);
  return newNode;
}

BinaryASTNodePtr RelationalExpressionContext::generateRelationalNode(
    SourceTokenType type, shared_ptr<ASTNode> leftNode) {
  BinaryASTNodePtr node;
  switch (type) {
    case SIMPLE_TOKEN_GT:
      node = make_shared<GtASTNode>();
      break;
    case SIMPLE_TOKEN_GTE:
      node = make_shared<GteASTNode>();
      break;
    case SIMPLE_TOKEN_LT:
      node = make_shared<LtASTNode>();
      break;
    case SIMPLE_TOKEN_LTE:
      node = make_shared<LteASTNode>();
      break;
    case SIMPLE_TOKEN_EQUALS:
      node = make_shared<EqualsASTNode>();
      break;
    case SIMPLE_TOKEN_NOT_EQUALS:
      node = make_shared<NotEqualsASTNode>();
      break;
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  node->setLeftChild(leftNode);
  return node;
}
