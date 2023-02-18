#include <memory>
#include "PrintContext.h"
#include "common/ASTNode/statement/CallNode.h"
#include "CallContext.h"

using std::shared_ptr, std::make_shared;

shared_ptr<ASTNode> CallContext::generateSubtree(SourceParseState* state) {
  // Call Node
  expect(state, SIMPLE_TOKEN_KEYWORD_CALL);

  // Name
  SourceToken* nameToken = expectVarchar(state);
  string procName = nameToken->getValue();

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<CallNode> callNode =
      make_shared<CallNode>(state->getLineNumber(), procName);
  state->setCached(callNode);
  return callNode;
}
