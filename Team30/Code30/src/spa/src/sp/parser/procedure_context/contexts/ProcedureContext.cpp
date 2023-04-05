#include <memory>
#include <utility>

#include "ProcedureContext.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::make_unique;

ASTNodePtr ProcedureContext::generateSubtree(SourceParseState *state) const {
  // Procedure Node
  state->expect(SIMPLE_TOKEN_KEYWORD_PROCEDURE);

  // Name
  SourceToken *nameToken = state->expectVarchar();
  ASTNodePtr procedureNode = make_unique<ProcedureNode>(nameToken->getValue());

  // Statement List
  ASTNodePtr stmtLst = contextProvider
      ->generateSubtree(ProcedureContextType::STMT_LIST_CONTEXT, state);
  procedureNode->addChild(std::move(stmtLst));
  return procedureNode;
}
