#include "IfContext.h"
#include "sp/common/ASTNode/IfNode.h"
#include "sp/common/ASTNode/ThenNode.h"

shared_ptr<ASTNode> IfContext::generateSubtree(SourceParseState* state) {
  shared_ptr<IfNode> ifNode = shared_ptr<IfNode>(new IfNode());
  expect(state, SIMPLE_TOKEN_KEYWORD_IF);

  // get conditional statement
  state->clearCached();
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
  shared_ptr<ASTNode> cond = contextProvider->
      getContext(COND_CONTEXT)->generateSubtree(state);
  expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);

  // then child
  expect(state, SIMPLE_TOKEN_KEYWORD_THEN);
  shared_ptr<ASTNode> thenNode = shared_ptr<ASTNode>(new ThenNode());
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    thenNode->addChild(newNode);
  }
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);

  // else child
  expect(state, SIMPLE_TOKEN_KEYWORD_ELSE);
  shared_ptr<ASTNode> elseNode = shared_ptr<ASTNode>(new ThenNode());
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  while (!state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    shared_ptr<ASTNode> newNode = contextProvider->
        getContext(STMT_CONTEXT)->generateSubtree(state);
    elseNode->addChild(newNode);
  }
  expect(state, SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  ifNode->setChild(0, cond);
  ifNode->setChild(1, thenNode);
  ifNode->setChild(2, elseNode);
  state->setCached(ifNode);
  return ifNode;
}

bool IfContext::validate(SourceParseState* state) {
  return true;
}
