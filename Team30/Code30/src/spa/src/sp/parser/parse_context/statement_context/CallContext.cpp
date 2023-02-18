#include <memory>
#include "common/ASTNode/statement/CallNode.h"
#include "CallContext.h"

using std::make_shared;

ASTNodePtr CallContext::generateSubtree(SourceParseState* state) {
  // Call Node
  expect(state, SIMPLE_TOKEN_KEYWORD_CALL);

  // Name
  SourceToken* nameToken = expectVarchar(state);
  string procName = nameToken->getValue();

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr callNode =
      make_shared<CallNode>(state->getLineNumber(), procName);
  state->setCached(callNode);
  return callNode;
}
