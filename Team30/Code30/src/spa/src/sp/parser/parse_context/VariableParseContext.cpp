#include <string>
#include <memory>
#include "VariableParseContext.h"
#include "common/ASTNode/entity/VariableASTNode.h"

using std::string, std::shared_ptr;

VariableParseContext::VariableParseContext() = default;

shared_ptr<ASTNode>
VariableParseContext::generateSubtree(SourceParseState* state) {
  SourceToken* token = expectVarchar(state);
  string value = token->getValue();
  shared_ptr<ASTNode> newNode =
      shared_ptr<ASTNode>(new VariableASTNode(value));
  return newNode;
}
