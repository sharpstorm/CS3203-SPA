#include "AssignContext.h"
#include "sp/common/ASTNode/statement/AssignNode.h"

shared_ptr<ASTNode> AssignContext::generateSubtree(SourceParseState* state) {
  // Assignee
  shared_ptr<ASTNode> name = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_ASSIGN);

  // Expression
  shared_ptr<ASTNode> expr = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<AssignNode> assignNode = shared_ptr<AssignNode>(new AssignNode());

  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  assignNode->lineNumber = state->getLineNumber();
  state->setCached(assignNode);
  return assignNode;
}
