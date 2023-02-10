#include "AssignContext.h"
#include "common/ASTNode/statement/AssignNode.h"

shared_ptr<ASTNode> AssignContext::generateSubtree(SourceParseState* state) {
  // Parse assignee TODO - change to varchar
  shared_ptr<ASTNode> name = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect Assign keyword
  expect(state, SIMPLE_TOKEN_ASSIGN);

  // Parse Expression
  shared_ptr<ASTNode> expr = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<AssignNode> assignNode = shared_ptr<AssignNode>(new AssignNode());
  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  assignNode->lineNumber = state->getLineNumber();
  state->setCached(assignNode);
  return assignNode;
}
