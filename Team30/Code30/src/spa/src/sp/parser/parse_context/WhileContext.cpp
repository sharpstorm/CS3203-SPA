#include "WhileContext.h"
#include "../../common/ASTNode/WhileNode.h"

shared_ptr<ASTNode> WhileContext::generateSubtree(SourceParseState* state) {
  shared_ptr<WhileNode> whileNode = shared_ptr<WhileNode>(new WhileNode());
  expect(state, SIMPLE_TOKEN_KEYWORD_WHILE);
  state->clearCached();
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  shared_ptr<ASTNode> cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
  whileNode->setChild(0, cond);
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    whileNode->addChild(newNode);
  }
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(whileNode);
  return whileNode;
}

bool WhileContext::validate(SourceParseState* state) {
  return true;
}
