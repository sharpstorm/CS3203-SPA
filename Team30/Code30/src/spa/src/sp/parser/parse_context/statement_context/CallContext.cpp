#include <memory>
#include "PrintContext.h"
#include "common/ASTNode/statement/CallNode.h"
#include "CallContext.h"

using std::shared_ptr;

shared_ptr<ASTNode> CallContext::generateSubtree(SourceParseState* state) {
  // Procedure Node
  expect(state, SIMPLE_TOKEN_KEYWORD_CALL);

  // Name
  SourceToken* nameToken = expectVarchar(state);
  shared_ptr<CallNode> callNode =
      shared_ptr<CallNode>(new CallNode(nameToken->getValue()));

  return callNode;
}
