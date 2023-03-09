#include <memory>

#include "ProcedureContext.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::make_shared;

ASTNodePtr ProcedureContext::generateSubtree(SourceParseState* state) {
  // Procedure Node
  state->expect(SIMPLE_TOKEN_KEYWORD_PROCEDURE);

  // Name
  SourceToken* nameToken = state->expectVarchar();
  ASTNodePtr procedureNode = make_shared<ProcedureNode>(nameToken->getValue());

  // Statement List
  ASTNodePtr stmtLst = contextProvider
      ->generateSubtree(ProcedureContextType::STMT_LIST_CONTEXT, state);

  procedureNode->addChild(stmtLst);
  state->setCached(procedureNode);
  return procedureNode;
}
