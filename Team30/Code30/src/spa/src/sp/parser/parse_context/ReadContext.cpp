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
  shared_ptr<ASTNode> printNode = shared_ptr<ASTNode>(new ReadNode());
  printNode->setChild(0, var);
  state->setCached(printNode);
  return printNode;
}

bool ReadContext::validate(SourceParseState* state) {
  return true;
}
