#include "IfContext.h"

#include <memory>
#include "sp/ast/statement/IfNode.h"

using std::make_shared;

ASTNodePtr IfContext::generateSubtree(SourceParseState* state) {
  // If Node
  state->expect(SIMPLE_TOKEN_KEYWORD_IF);
  ASTNodePtr ifNode = make_shared<IfNode>(
      state->getLineNumber());

  // Conditional Expression
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  ASTNodePtr cond = contextProvider->parseCondition(state);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // Then StmtLst
  state->expect(SIMPLE_TOKEN_KEYWORD_THEN);
  ASTNodePtr thenStmtLst = contextProvider
      ->generateSubtree(ProcedureContextType::STMT_LIST_CONTEXT, state);

  // Else StmtLst
  state->expect(SIMPLE_TOKEN_KEYWORD_ELSE);
  ASTNodePtr elseStmtLst = contextProvider
      ->generateSubtree(ProcedureContextType::STMT_LIST_CONTEXT, state);

  ifNode->setChild(0, cond);
  ifNode->setChild(1, thenStmtLst);
  ifNode->setChild(2, elseStmtLst);
  state->setCached(ifNode);
  return ifNode;
}
