#include "IfContext.h"

#include <memory>
#include "common/ast/statement/IfNode.h"
#include "common/ast/StatementListNode.h"

using std::make_shared;

ASTNodePtr IfContext::generateSubtree(SourceParseState* state) {
  // If Node
  expect(state, SIMPLE_TOKEN_KEYWORD_IF);
  ASTNodePtr ifNode = make_shared<IfNode>(
      state->getLineNumber());

  // Conditional Expression
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  ASTNodePtr cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // Then StmtLst
  expect(state, SIMPLE_TOKEN_KEYWORD_THEN);
  ASTNodePtr thenStmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  // Else StmtLst
  expect(state, SIMPLE_TOKEN_KEYWORD_ELSE);
  ASTNodePtr elseStmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  ifNode->setChild(0, cond);
  ifNode->setChild(1, thenStmtLst);
  ifNode->setChild(2, elseStmtLst);
  state->setCached(ifNode);
  return ifNode;
}
