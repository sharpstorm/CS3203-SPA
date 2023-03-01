#include <memory>
#include "ProgramContext.h"
#include "common/ast/entity/ProgramNode.h"

using std::make_shared;

ASTNodePtr ProgramContext::generateSubtree(SourceParseState* state) {
  ASTNodePtr programNode = make_shared<ProgramNode>();
  while (!state->isEnd()) {
    programNode->addChild(contextProvider->
        getContext(PROCEDURE_CONTEXT)->
        generateSubtree(state));
  }
  state->setCached(programNode);
  return programNode;
}
