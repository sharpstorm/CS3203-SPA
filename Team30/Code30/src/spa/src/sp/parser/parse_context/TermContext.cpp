#include "TermContext.h"
#include "../../common/ASTNode/math/PlusASTNode.h"
#include "sp/common/ASTNode/math/DivASTNode.h"
#include "sp/common/ASTNode/math/ModASTNode.h"

bool TermContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> TermContext::generateSubtree(SourceParseState *state) {


  if (state->isAtLast()) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }

  // if current constant / variable is followed by a *, /, % or ;, return constant / variable
  if (state->peekNextToken()->isType(SIMPLE_TOKEN_TIMES) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_DIV) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_MOD) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_SEMICOLON) ||
          state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }

  // if current token is factor integer, create node and return it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_INTEGER) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_VARIABLE) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT))  {
    shared_ptr<ASTNode> node = contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
    return node;
  }

  bool toReRoot = state->getCached()->toString().at(0) == '+' ||
                  state->getCached()->toString().at(0) == '-';

  shared_ptr<ASTNode> leftNode;
  leftNode = toReRoot ? state->getCached()->getChildren().at(1) : state->getCached();

  shared_ptr<AbstractMathASTNode> middleNode;
  SourceToken* token = expect(state, SIMPLE_TOKEN_TIMES, SIMPLE_TOKEN_DIV, SIMPLE_TOKEN_MOD);

  switch(token->getType()) {
    case SIMPLE_TOKEN_TIMES:
      middleNode = generateTimes(state, leftNode);
      break;
    case SIMPLE_TOKEN_DIV:
      middleNode = generateDiv(state, leftNode);
      break;
    case SIMPLE_TOKEN_MOD:
      middleNode = generateMod(state, leftNode);
      break;
  }

  // re-root
  if (toReRoot) {
    state->getCached()->setChild(1, middleNode);
    middleNode->setRightChild(contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
  } else {
    state->setCached(middleNode);
    middleNode->setRightChild(contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
  }

  // if next is a + / - perform EXPR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_PLUS) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MINUS) ) {
    return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }

  // if next is a + / - perform TERM_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_DIV) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MOD)) {
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  }

  // if next is a '(' perform FACTOR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }
  return middleNode;
}

shared_ptr<AbstractMathASTNode> TermContext::generateTimes(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  TimesASTNode* node = new TimesASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<TimesASTNode> newNode = shared_ptr<TimesASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode> TermContext::generateDiv(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  DivASTNode* node = new DivASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<DivASTNode> newNode = shared_ptr<DivASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode> TermContext::generateMod(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  ModASTNode* node = new ModASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<ModASTNode> newNode = shared_ptr<ModASTNode>(node);
  return newNode;
}

