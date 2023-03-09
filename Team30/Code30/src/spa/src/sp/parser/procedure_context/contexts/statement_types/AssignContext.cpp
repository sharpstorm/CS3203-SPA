#include "AssignContext.h"

#include <memory>
#include "sp/ast/statement/AssignNode.h"

using std::make_shared;

ASTNodePtr AssignContext::generateSubtree(SourceParseState* state) {
  // Parse assignee
  ASTNodePtr name = contextProvider->parseVariable(state);

  // Expect Assign operator
  state->expect(SIMPLE_TOKEN_ASSIGN);

  // Parse Expression
  ASTNodePtr expr = contextProvider->parseExpression(state);
  state->expect(SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr assignNode = make_shared<AssignNode>(state->getLineNumber());
  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  state->setCached(assignNode);
  return assignNode;
}
