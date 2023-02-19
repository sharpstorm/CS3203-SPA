#include <string>
#include <memory>

#include "ConstantParseContext.h"
#include "common/ASTNode/entity/ConstantASTNode.h"

using std::string, std::make_shared;

ConstantParseContext::ConstantParseContext() = default;

ASTNodePtr ConstantParseContext::generateSubtree(SourceParseState* state) {
  SourceToken* token = expect(state, SIMPLE_TOKEN_INTEGER);
  string value = token->getValue();
  ASTNodePtr newNode = make_shared<ConstantASTNode>(value);
  if (!state->hasCached()) {
    state->setCached(newNode);
  }

  return newNode;
}
