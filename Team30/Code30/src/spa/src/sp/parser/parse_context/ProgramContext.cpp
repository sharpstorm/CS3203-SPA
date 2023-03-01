#include <memory>
#include "ProgramContext.h"
#include "common/ast/entity/ProgramNode.h"

using std::make_shared;

ASTNodePtr ProgramContext::generateSubtree(SourceParseState* state) {
  ASTNodePtr programNode = make_shared<ProgramNode>();
  addProcedure(programNode, state);
  while (!state->isEnd()) {
    addProcedure(programNode, state);
  }
  state->setCached(programNode);
  return programNode;
}

void ProgramContext::addProcedure(ASTNodePtr node, SourceParseState* state) {
  node->addChild(contextProvider->
      getContext(PROCEDURE_CONTEXT)->
      generateSubtree(state));
}