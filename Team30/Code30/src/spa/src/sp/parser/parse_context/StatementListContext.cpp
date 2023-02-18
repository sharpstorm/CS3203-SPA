#include <memory>
#include "common/ASTNode/StatementListNode.h"
#include "StatementListContext.h"

using std::shared_ptr;

shared_ptr<ASTNode> StatementListContext::generateSubtree(
    SourceParseState *state) {
  shared_ptr<StatementListNode> node =
      shared_ptr<StatementListNode>(new StatementListNode());

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    node->addChild(newNode);
    if (state->isAtLast() || state->isEnd()) {
      break;
    }
  }

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(node);

  return node;
}
