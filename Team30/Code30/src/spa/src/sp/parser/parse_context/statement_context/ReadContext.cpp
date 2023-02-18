#include <memory>
#include "ReadContext.h"
#include "common/ASTNode/statement/ReadNode.h"

using std::shared_ptr, std::make_shared;

shared_ptr<ASTNode> ReadContext::generateSubtree(SourceParseState* state) {
  // Expect 'read'
  expect(state, SIMPLE_TOKEN_KEYWORD_READ);

  // Parse varchar
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<ReadNode> readNode = make_shared<ReadNode>(
      state->getLineNumber());
  readNode->setChild(0, var);
  state->setCached(readNode);
  return readNode;
}

