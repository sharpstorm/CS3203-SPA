#include <memory>
#include "StatementContext.h"
#include "../../errors/SPError.h"
#include "sp/common/ASTNode/StatementListNode.h"
#include "StatementListContext.h"

using std::shared_ptr;

bool StatementListContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> StatementListContext::generateSubtree(
    SourceParseState *state) {
  shared_ptr<StatementListNode> node =
      shared_ptr<StatementListNode>(new StatementListNode());

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    node->addChild(newNode);
  }

  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(node);

  return node;
}
