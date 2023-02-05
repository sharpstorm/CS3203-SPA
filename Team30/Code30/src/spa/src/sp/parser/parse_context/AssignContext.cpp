#include "AssignContext.h"
#include "../../common/ASTNode/AssignNode.h"

shared_ptr<ASTNode> AssignContext::generateSubtree(SourceParseState* state) {
  shared_ptr<ASTNode> name = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_ASSIGN);
  state->clearCached();
  shared_ptr<ASTNode> expr = contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<ASTNode> assignNode = shared_ptr<ASTNode>(new AssignNode());
  assignNode->setChild(0, name);
  assignNode->setChild(1, expr);
  state->setCached(assignNode);
  assignNode->lineNumber = contextProvider->currLineCounter();
  name->lineNumber = contextProvider->currLineCounter();
  expr->setAllLine(contextProvider->currLineCounter());
  contextProvider->advanceLineCounter();
  return assignNode;
}

bool AssignContext::validate(SourceParseState* state) {
  return true;
}
