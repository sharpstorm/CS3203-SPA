#include "AssignContext.h"
#include "common/ASTNode/statement/AssignNode.h"

using std::make_shared;

shared_ptr<ASTNode> AssignContext::generateSubtree(SourceParseState* state) {
  // Parse assignee
  shared_ptr<ASTNode> name = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect Assign keyword
  expect(state, SIMPLE_TOKEN_ASSIGN);

  // Parse Expression
  shared_ptr<ASTNode> expr = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<AssignNode> assignNode = make_shared<AssignNode>(
      state->getLineNumber());
  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  state->setCached(assignNode);
  return assignNode;
}
