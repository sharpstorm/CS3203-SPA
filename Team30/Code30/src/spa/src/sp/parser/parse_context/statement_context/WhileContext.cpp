#include "WhileContext.h"

#include <memory>
#include "common/ASTNode/statement/WhileNode.h"

using std::make_shared;

ASTNodePtr WhileContext::generateSubtree(SourceParseState* state) {
  // While Node
  expect(state, SIMPLE_TOKEN_KEYWORD_WHILE);
  ASTNodePtr whileNode = make_shared<WhileNode>(
      state->getLineNumber());

  // Conditional Expression
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  ASTNodePtr cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // Statement List
  ASTNodePtr stmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  // Assign to children to While Node
  whileNode->setChild(0, cond);
  whileNode->setChild(1, stmtLst);
  state->setCached(whileNode);
  return whileNode;
}
