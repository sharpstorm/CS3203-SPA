#include <memory>
#include "ReadContext.h"
#include "sp/common/ASTNode/statement/ReadNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ReadContext::generateSubtree(SourceParseState* state) {
  expect(state, SIMPLE_TOKEN_KEYWORD_READ);

  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<ReadNode> readNode = shared_ptr<ReadNode>(new ReadNode());
  readNode->setChild(0, var);
  state->setCached(readNode);
  readNode->lineNumber = state->getLineNumber();
  return readNode;
}

bool ReadContext::validate(SourceParseState* state) {
  return true;
}
