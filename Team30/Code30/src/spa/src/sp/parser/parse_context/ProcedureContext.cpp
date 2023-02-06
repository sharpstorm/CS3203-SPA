#include <memory>
#include "ProcedureContext.h"
#include "../../common/ASTNode/ProcedureNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ProcedureContext::generateSubtree(SourceParseState* state) {
  // Procedure Node
  expect(state, SIMPLE_TOKEN_KEYWORD_PROCEDURE);

  // Name
  SourceToken* nameToken = expectVarchar(state);
  shared_ptr<ProcedureNode> procedureNode =
      shared_ptr<ProcedureNode>(new ProcedureNode(nameToken->getValue()));

  // Statement List
  shared_ptr<ASTNode> stmtLst = contextProvider->
      getContext(STMT_LIST_CONTEXT)->generateSubtree(state);

  procedureNode->addChild(stmtLst);
  state->setCached(procedureNode);
  return procedureNode;
}

bool ProcedureContext::validate(SourceParseState* state) {
  return true;
}
