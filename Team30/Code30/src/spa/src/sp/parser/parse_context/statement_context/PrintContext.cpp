#include <memory>
#include "PrintContext.h"
#include "common/ASTNode/statement/PrintNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> PrintContext::generateSubtree(SourceParseState* state) {
  // Expect 'print'
  expect(state, SIMPLE_TOKEN_KEYWORD_PRINT);

  // Parse varchar TODO - change to varchar
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  //expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<PrintNode> printNode = shared_ptr<PrintNode>(new PrintNode());
  printNode->setChild(0, var);
  state->setCached(printNode);
  printNode->lineNumber = state->getLineNumber();
  return printNode;
}
