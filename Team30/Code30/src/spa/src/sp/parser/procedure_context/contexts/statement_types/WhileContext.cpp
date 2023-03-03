#include "WhileContext.h"

#include <memory>
#include "common/ast/statement/WhileNode.h"

using std::make_shared;

ASTNodePtr WhileContext::generateSubtree(SourceParseState* state) {
  // While Node
  state->expect(SIMPLE_TOKEN_KEYWORD_WHILE);
  ASTNodePtr whileNode = make_shared<WhileNode>(
      state->getLineNumber());

  // Conditional Expression
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  ASTNodePtr cond = contextProvider->getConditionalParser()->parse(state);
  state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // Statement List
  ASTNodePtr stmtLst = contextProvider
      ->getContext(ProcedureContextType::STMT_LIST_CONTEXT)
      ->generateSubtree(state);

  // Assign to children to While Node
  whileNode->setChild(0, cond);
  whileNode->setChild(1, stmtLst);
  state->setCached(whileNode);
  return whileNode;
}
