#include <memory>
#include "PrintContext.h"
#include "../../common/ASTNode/PrintNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> PrintContext::generateSubtree(SourceParseState* state) {
  expect(state, SIMPLE_TOKEN_KEYWORD_PRINT);
  state->clearCached();
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<ASTNode> printNode = shared_ptr<ASTNode>(new PrintNode());
  printNode->setChild(0, var);
  state->setCached(printNode);
  printNode->lineNumber = contextProvider->currLineCounter();
  var->lineNumber = contextProvider->currLineCounter();
  contextProvider->advanceLineCounter();
  return printNode;
}

bool PrintContext::validate(SourceParseState* state) {
  return true;
}
