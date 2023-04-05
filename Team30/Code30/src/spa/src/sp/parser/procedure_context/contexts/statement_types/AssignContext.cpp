#include "AssignContext.h"

#include <memory>
#include <utility>

#include "sp/ast/statement/AssignNode.h"

using std::make_unique;

ASTNodePtr AssignContext::generateSubtree(SourceParseState *state) const {
  // Parse assignee
  ASTNodePtr name = contextProvider->parseVariable(state);

  // Expect Assign operator
  state->expect(SIMPLE_TOKEN_ASSIGN);

  // Parse Expression
  ASTNodePtr expr = contextProvider->parseExpression(state);
  state->expect(SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr assignNode = make_unique<AssignNode>(state->getLineNumber());
  assignNode->setChild(0, std::move(name));
  assignNode->setChild(1, std::move(expr));
  return assignNode;
}
