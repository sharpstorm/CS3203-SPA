#include <memory>
#include "ProcedureContext.h"
#include "../../common/ASTNode/ProcedureNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ProcedureContext::generateSubtree(SourceParseState* state) {
  shared_ptr<ProcedureNode> procedureNode =
      shared_ptr<ProcedureNode>(new ProcedureNode());
  expect(state, SIMPLE_TOKEN_KEYWORD_PROCEDURE);

  // dont think this is needed but let me jus leave it here :]
  state->clearCached();

  procedureNode->addChild(contextProvider->
  getContext(VARIABLE_CONTEXT)->generateSubtree(state));
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    procedureNode->addChild(newNode);
  }
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(procedureNode);
  return procedureNode;
}

bool ProcedureContext::validate(SourceParseState* state) {
  return true;
}
