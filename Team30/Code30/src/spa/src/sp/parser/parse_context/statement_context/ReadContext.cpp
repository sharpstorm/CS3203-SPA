#include "ReadContext.h"

#include <memory>
#include "common/ASTNode/statement/ReadNode.h"

using std::make_shared;

ASTNodePtr ReadContext::generateSubtree(SourceParseState* state) {
  // Expect 'read'
  expect(state, SIMPLE_TOKEN_KEYWORD_READ);

  // Parse varchar
  ASTNodePtr var = contextProvider->
      getContext(VARIABLE_CONTEXT)->generateSubtree(state);

  // Expect ';'
  expect(state, SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr readNode = make_shared<ReadNode>(state->getLineNumber());
  readNode->setChild(0, var);
  state->setCached(readNode);
  return readNode;
}

