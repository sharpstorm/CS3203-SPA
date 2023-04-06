#include "IfContext.h"

#include <memory>
#include <utility>
#include "sp/ast/statement/IfNode.h"

using std::make_unique;

ASTNodePtr IfContext::generateSubtree(SourceParseState *state) const {
  // If Node
  state->expect(SIMPLE_TOKEN_KEYWORD_IF);
  ASTNodePtr ifNode = make_unique<IfNode>(state->getLineNumber());

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

  ifNode->setChild(0, std::move(cond));
  ifNode->setChild(1, std::move(thenStmtLst));
  ifNode->setChild(2, std::move(elseStmtLst));
  return ifNode;
}
