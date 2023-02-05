#include <memory>
#include "ExpressionContext.h"

using std::shared_ptr;

bool ExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ExpressionContext::generateSubtree(
    SourceParseState *state) {

  if (!state->hasCached()) {
    shared_ptr<ASTNode>firstNode = (contextProvider->
        getContext(FACTOR_CONTEXT)->generateSubtree(state));
    state->setCached(firstNode);
    if (state->isAtLast()) {
      return firstNode;
    }
    if (!state->getCurrToken()->isType(SIMPLE_TOKEN_PLUS) &&
        !state->getCurrToken()->isType(SIMPLE_TOKEN_MINUS) &&
        !state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES) &&
        !state->getCurrToken()->isType(SIMPLE_TOKEN_DIV) &&
        !state->getCurrToken()->isType(SIMPLE_TOKEN_MOD)) {
      return firstNode;
    }
  }

  // if is last character of expression, must be a factor
  if (state->isAtLast()) {
    return contextProvider->getContext(FACTOR_CONTEXT)->
        generateSubtree(state);
  }

  // if next constant / variable is not a factor
  if (!state->peekNextToken()->isType(SIMPLE_TOKEN_INTEGER) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_VARIABLE) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT)) {
    return contextProvider->getContext(TERM_CONTEXT)->
        generateSubtree(state);
  }

  // Throw expression to term context
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES)  ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_DIV)  ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MOD)) {
    shared_ptr<ASTNode> node = contextProvider->
        getContext(TERM_CONTEXT)->generateSubtree(state);
    return node;
  }

  // if expression is a factor of type '(' expr ')'
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_INTEGER) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_VARIABLE)) {
    shared_ptr<ASTNode> node = contextProvider->
        getContext(FACTOR_CONTEXT)->generateSubtree(state);
    return node;
  }

  // create + / - AST node
  shared_ptr<ASTNode> leftNode = state->getCached();
  shared_ptr<AbstractMathASTNode> middleNode;
  SourceToken* token = expect(state, SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS);

  if (token->isType(SIMPLE_TOKEN_PLUS)) {
    middleNode = generatePlus(state, leftNode);
  } else if (token->isType(SIMPLE_TOKEN_MINUS)) {
    middleNode = generateMinus(state, leftNode);
  }

  state->setCached(middleNode);
  middleNode->setRightChild(contextProvider->
      getContext(EXPR_CONTEXT)->generateSubtree(state));

  // If next token is a + / - perform EXPR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_PLUS) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MINUS) ) {
    return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }

  // if next is a * / '/' / % perform EXPR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES)  ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_DIV)  ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MOD)) {
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  }

  return middleNode;
}

shared_ptr<PlusASTNode> ExpressionContext::generatePlus(
    SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  PlusASTNode* node = new PlusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<PlusASTNode> newNode = shared_ptr<PlusASTNode>(node);
  return newNode;
}

shared_ptr<MinusASTNode> ExpressionContext::generateMinus(
    SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  MinusASTNode* node = new MinusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<MinusASTNode> newNode = shared_ptr<MinusASTNode>(node);
  return newNode;
}
