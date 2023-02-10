#include "IfContext.h"
#include "sp/common/ASTNode/statement/IfNode.h"
#include "sp/common/ASTNode/StatementListNode.h"

shared_ptr<ASTNode> IfContext::generateSubtree(SourceParseState* state) {
  // If Node
  expect(state, SIMPLE_TOKEN_KEYWORD_IF);
  shared_ptr<IfNode> ifNode = shared_ptr<IfNode>(new IfNode());
  ifNode->lineNumber = state->getLineNumber();

  // Conditional Expression
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  shared_ptr<ASTNode> cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);


  // Then StmtLst
  expect(state, SIMPLE_TOKEN_KEYWORD_THEN);
  shared_ptr<ASTNode> thenStmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  // Else StmtLst
  expect(state, SIMPLE_TOKEN_KEYWORD_ELSE);
  shared_ptr<ASTNode> elseStmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  ifNode->setChild(0, cond);
  ifNode->setChild(1, thenStmtLst);
  ifNode->setChild(2, elseStmtLst);
  state->setCached(ifNode);
  return ifNode;
}

