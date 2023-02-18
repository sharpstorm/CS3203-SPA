#include <memory>
#include "PrintContext.h"
#include "common/ASTNode/statement/PrintNode.h"

using std::shared_ptr, std::make_shared;

shared_ptr<ASTNode> PrintContext::generateSubtree(SourceParseState* state) {
  // Expect 'print'
  expect(state, SIMPLE_TOKEN_KEYWORD_PRINT);

  // Parse varchar
  shared_ptr<ASTNode> var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  shared_ptr<PrintNode> printNode = make_shared<PrintNode>(
      state->getLineNumber());
  printNode->setChild(0, var);
  state->setCached(printNode);
  return printNode;
}
