#include <memory>
#include "PrintContext.h"
#include "sp/common/ASTNode/statement/PrintNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> PrintContext::generateSubtree(SourceParseState* state) {
  expect(state, SIMPLE_TOKEN_KEYWORD_PRINT);

  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_SEMICOLON);
  shared_ptr<PrintNode> printNode = shared_ptr<PrintNode>(new PrintNode());
  printNode->setChild(0, var);
  state->setCached(printNode);
  printNode->lineNumber = state->getLineNumber();
  return printNode;
}

bool PrintContext::validate(SourceParseState* state) {
  return true;
}
