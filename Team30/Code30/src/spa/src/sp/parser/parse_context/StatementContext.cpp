#include "StatementContext.h"
#include "../../errors/SPError.h"
#include "../../common/ASTNode/struct/AssignASTNode.h"

bool StatementContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> StatementContext::generateSubtree(SourceParseState *state) {
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_VARIABLE:
      return generateAssign(state);
    case SIMPLE_TOKEN_KEYWORD_CALL:
      throw SPError("Not implemented");
    default:
      throw SPError("Unknown token sequence");
  }
}

shared_ptr<ASTNode> StatementContext::generateAssign(SourceParseState* state) {
  shared_ptr<ASTNode> leftNode = contextProvider->getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_ASSIGN);

  state->clearCached();
  shared_ptr<ASTNode> rightNode = contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);

  AssignASTNode* assignNode = new AssignASTNode();
  assignNode->setLeftChild(leftNode);
  assignNode->setRightChild(rightNode);
  shared_ptr<ASTNode> newNode = shared_ptr<ASTNode>(assignNode);
  return newNode;
}
