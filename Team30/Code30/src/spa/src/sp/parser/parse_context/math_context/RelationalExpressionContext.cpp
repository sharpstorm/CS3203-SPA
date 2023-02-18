#include <memory>
#include "RelationalExpressionContext.h"
#include "common/ASTNode/math/conditional_operand/GtASTNode.h"
#include "common/ASTNode/math/conditional_operand/GteASTNode.h"
#include "common/ASTNode/math/conditional_operand/LteASTNode.h"
#include "common/ASTNode/math/conditional_operand/EqualsASTNode.h"
#include "common/ASTNode/math/conditional_operand/NotEqualsASTNode.h"

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
  shared_ptr<BinaryASTNode> newNode =
      generateRelationalNode(token->getType(), leftNode);
  newNode->setRightChild(contextProvider->
      getContext(REL_FACTOR_CONTEXT)->generateSubtree(state));
  state->setCached(newNode);
  return newNode;
}

shared_ptr<BinaryASTNode>
RelationalExpressionContext::generateRelationalNode
    (SourceTokenType type, shared_ptr<ASTNode> leftNode) {
  shared_ptr<BinaryASTNode> node;
  switch (type) {
    case SIMPLE_TOKEN_GT:
      node = shared_ptr<BinaryASTNode>(new GtASTNode());
      break;
    case SIMPLE_TOKEN_GTE:
      node = shared_ptr<BinaryASTNode>(new GteASTNode());
      break;
    case SIMPLE_TOKEN_LT:
      node = shared_ptr<BinaryASTNode>(new GtASTNode());
      break;
    case SIMPLE_TOKEN_LTE:
      node = shared_ptr<BinaryASTNode>(new LteASTNode());
      break;
    case SIMPLE_TOKEN_EQUALS:
      node = shared_ptr<BinaryASTNode>(new EqualsASTNode());
      break;
    case SIMPLE_TOKEN_NOT_EQUALS:
      node = shared_ptr<BinaryASTNode>(new NotEqualsASTNode());
      break;
    default:
      throw SPError("Unknown token");
  }
  node->setLeftChild(leftNode);
  return node;
}
