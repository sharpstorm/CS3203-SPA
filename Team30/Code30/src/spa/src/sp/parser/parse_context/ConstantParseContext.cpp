#include <string>
#include "ConstantParseContext.h"
#include "../../common/ASTNode/ConstantASTNode.h"

using std::string;

ConstantParseContext::ConstantParseContext() = default;

shared_ptr<ASTNode>ConstantParseContext::
generateSubtree(SourceParseState* state) {
  SourceToken* token = expect(state, SIMPLE_TOKEN_INTEGER);
  string value = token->getValue();
  shared_ptr<ASTNode> newNode = shared_ptr<ASTNode>(
      new ConstantASTNode(value));
  if (!state->hasCached()) {
    state->setCached(newNode);
  }
  return newNode;
}
