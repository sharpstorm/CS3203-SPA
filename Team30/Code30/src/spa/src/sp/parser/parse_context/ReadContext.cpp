#include <memory>
#include "ReadContext.h"
#include "../../common/ASTNode/ReadNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> ReadContext::generateSubtree(SourceParseState* state) {
  expect(state, SIMPLE_TOKEN_KEYWORD_READ);
  state->clearCached();
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<ASTNode> readNode = shared_ptr<ASTNode>(new ReadNode());
  readNode->setChild(0, var);
  state->setCached(readNode);
  readNode->lineNumber = contextProvider->currLineCounter();
  var->lineNumber = contextProvider->currLineCounter();
  contextProvider->advanceLineCounter();
  return readNode;
}

bool ReadContext::validate(SourceParseState* state) {
  return true;
}
