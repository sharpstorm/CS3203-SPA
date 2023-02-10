#include "WhileContext.h"
#include "common/ASTNode/statement/WhileNode.h"

shared_ptr<ASTNode> WhileContext::generateSubtree(SourceParseState* state) {
  shared_ptr<WhileNode> whileNode = shared_ptr<WhileNode>(new WhileNode());
  // While Node
  expect(state, SIMPLE_TOKEN_KEYWORD_WHILE);
  whileNode->lineNumber = state->getLineNumber();

  // Conditional Expression
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  shared_ptr<ASTNode> cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // Statement List
  shared_ptr<ASTNode> stmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  // Assign to children to While Node
  whileNode->setChild(0, cond);
  whileNode->setChild(1, stmtLst);
  state->setCached(whileNode);
  return whileNode;
}
