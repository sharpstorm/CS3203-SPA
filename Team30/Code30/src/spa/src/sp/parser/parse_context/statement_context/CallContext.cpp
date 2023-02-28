#include "CallContext.h"

#include <memory>
#include <string>
#include "common/ast/statement/CallNode.h"

using std::make_shared, std::string;

ASTNodePtr CallContext::generateSubtree(SourceParseState* state) {
  // Call Node
  expect(state, SIMPLE_TOKEN_KEYWORD_CALL);

  // Name
  SourceToken* nameToken = expectVarchar(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  string procName = nameToken->getValue();
  ASTNodePtr callNode =
      make_shared<CallNode>(state->getLineNumber(), procName);
  state->setCached(callNode);
  return callNode;
}
