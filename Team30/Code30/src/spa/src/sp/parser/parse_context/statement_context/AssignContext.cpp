#include "AssignContext.h"

#include <memory>
#include "common/ASTNode/statement/AssignNode.h"

using std::make_shared;

ASTNodePtr AssignContext::generateSubtree(SourceParseState* state) {
  // Parse assignee
  ASTNodePtr name = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect Assign keyword
  expect(state, SIMPLE_TOKEN_ASSIGN);

  // Parse Expression
  ASTNodePtr expr = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);

  expect(state, SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr assignNode = make_shared<AssignNode>(state->getLineNumber());
  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  state->setCached(assignNode);
  return assignNode;
}
