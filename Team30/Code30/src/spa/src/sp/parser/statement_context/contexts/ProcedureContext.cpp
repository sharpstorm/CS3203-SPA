#include <memory>
#include "ProcedureContext.h"
#include "common/ast/entity/ProcedureNode.h"

using std::make_shared;

ASTNodePtr ProcedureContext::generateSubtree(SourceParseState* state) {
  // Procedure Node
  expect(state, SIMPLE_TOKEN_KEYWORD_PROCEDURE);

  // Name
  SourceToken* nameToken = expectVarchar(state);
  ASTNodePtr procedureNode = make_shared<ProcedureNode>(nameToken->getValue());

  // Statement List
  ASTNodePtr stmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  procedureNode->addChild(stmtLst);
  state->setCached(procedureNode);
  return procedureNode;
}
