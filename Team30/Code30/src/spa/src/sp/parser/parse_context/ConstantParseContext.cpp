#include "ConstantParseContext.h"
#include "../../errors/SPError.h"
#include "../../common/ASTNode/ConstantASTNode.h"

ConstantParseContext::ConstantParseContext() = default;

shared_ptr<ASTNode> ConstantParseContext::generateSubtree(SourceParseState* state) {
  SourceToken* token = expect(state, SIMPLE_TOKEN_INTEGER);
  string value = token->getValue();

  if (!validate(state)) {
    throw SPError("Invalid constant: " + value);
  }

  shared_ptr<ASTNode> newNode = shared_ptr<ASTNode>(
      new ConstantASTNode(value));

  if (!state->hasCached()) {
      state->setCached(newNode);
  }

  return newNode;
}

bool ConstantParseContext::validate(SourceParseState* state) {
  return true;
}
