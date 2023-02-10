#include <memory>
#include "ReadContext.h"
#include "common/ASTNode/statement/ReadNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ReadContext::generateSubtree(SourceParseState* state) {
  // Expect 'read'
  expect(state, SIMPLE_TOKEN_KEYWORD_READ);

  // Parse varchar TODO - change to varchar
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<ReadNode> readNode = shared_ptr<ReadNode>(new ReadNode());
  readNode->setChild(0, var);
  state->setCached(readNode);
  readNode->lineNumber = state->getLineNumber();
  return readNode;
}

