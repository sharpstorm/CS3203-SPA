#include <memory>
#include "common/ASTNode/StatementListNode.h"
#include "StatementListContext.h"

using std::make_shared;

ASTNodePtr StatementListContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr node = make_shared<StatementListNode>();

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    ASTNodePtr newNode = contextProvider->
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
