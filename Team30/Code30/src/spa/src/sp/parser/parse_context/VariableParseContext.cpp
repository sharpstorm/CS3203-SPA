#include "VariableParseContext.h"
#include "../../common/ASTNode/VariableASTNode.h"

VariableParseContext::VariableParseContext() = default;

shared_ptr<ASTNode> VariableParseContext::generateSubtree(SourceParseState* state) {
  SourceToken* token = expect(state, SIMPLE_TOKEN_VARIABLE);
  string value = token->getValue();
  if (!validate(state)) {
    throw SPError("Invalid constant: " + value);
  }

  shared_ptr<ASTNode> newNode = shared_ptr<ASTNode>(new VariableASTNode(value));
  return newNode;
}

bool VariableParseContext::validate(SourceParseState* state) {
  return true;
}
