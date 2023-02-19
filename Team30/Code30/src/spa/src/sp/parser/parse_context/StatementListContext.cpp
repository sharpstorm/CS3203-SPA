#include <memory>
#include "common/ASTNode/StatementListNode.h"
#include "StatementListContext.h"

using std::make_shared;

ASTNodePtr StatementListContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr node = make_shared<StatementListNode>();

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  ASTNodePtr newNode = contextProvider->
      getContext(STMT_CONTEXT)->generateSubtree(state);
  node->addChild(newNode);

  while (!state->currTokenIsOfType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    node->addChild(newNode);
  }

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(node);

  return node;
}
