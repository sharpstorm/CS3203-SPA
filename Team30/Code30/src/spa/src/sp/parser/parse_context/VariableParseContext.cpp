#include <string>
#include <memory>
#include "VariableParseContext.h"
#include "common/ASTNode/entity/VariableASTNode.h"

using std::string, std::make_shared;

VariableParseContext::VariableParseContext() = default;

ASTNodePtr VariableParseContext::generateSubtree(SourceParseState* state) {
  SourceToken* token = expectVarchar(state);
  string value = token->getValue();
  shared_ptr<ASTNode> newNode = make_shared<VariableASTNode>(value);
  return newNode;
}
