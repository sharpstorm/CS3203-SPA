#include "PrintContext.h"

#include <memory>
#include "common/ast/statement/PrintNode.h"

using std::make_shared;

ASTNodePtr PrintContext::generateSubtree(SourceParseState* state) {
  // Expect 'print'
  expect(state, SIMPLE_TOKEN_KEYWORD_PRINT);

  // Parse varchar
  ASTNodePtr var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr printNode = make_shared<PrintNode>(state->getLineNumber());
  printNode->setChild(0, var);
  state->setCached(printNode);
  return printNode;
}
