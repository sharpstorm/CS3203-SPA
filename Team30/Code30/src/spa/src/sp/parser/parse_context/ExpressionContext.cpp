#include "ExpressionContext.h"

#include "../../common/ASTNode/math/PlusASTNode.h"
#include <iostream>
bool ExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ExpressionContext::generateSubtree(SourceParseState *state) {
  // each expression should be parsed with a state
  if (!state->hasCached()) {
    // I'm first
    shared_ptr<ASTNode>firstNode = (contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
    state->setCached(firstNode);
    //handles single digit/var expressions such as '1;'
    if (state->getCurrToken()->isType(SIMPLE_TOKEN_SEMICOLON)) {
      return firstNode;
    }
  }

  // if current constant / variable is followed by , return constant / variable
  if (state->peekNextToken()->isType(SIMPLE_TOKEN_PLUS) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_MINUS) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_TIMES) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_SEMICOLON) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }

  // if expression is a term
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES)) {
    shared_ptr<ASTNode> node = contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
    return node;
  }

  // if expression is a factor of type '(' expr ')'
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_BRACKET_ROUND_LEFT)) {
    shared_ptr<ASTNode> node = contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
    return node;
  }

  // create + / - AST node
  shared_ptr<ASTNode> leftNode = state->getCached();
  shared_ptr<AbstractMathASTNode> middleNode;
  SourceToken* token = expect(state, SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS);

  if (token->isType(SIMPLE_TOKEN_PLUS)) {
    middleNode = generatePlus(state, leftNode);
  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_MINUS)) {
    middleNode = generateMinus(state, leftNode);
  }

  state->setCached(middleNode);
  middleNode->setRightChild(contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state));

  // if next is a + / - perform EXPR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_PLUS) ||
      state->getCurrToken()->isType(SIMPLE_TOKEN_MINUS) ) {
      return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }

  // if next is a * / '/' / % perform EXPR_CONTEXT on it
  if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES)) {
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  }

  return middleNode;
}

shared_ptr<PlusASTNode> ExpressionContext::generatePlus(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  PlusASTNode* node = new PlusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<PlusASTNode> newNode = shared_ptr<PlusASTNode>(node);
  return newNode;
}

shared_ptr<MinusASTNode> ExpressionContext::generateMinus(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  MinusASTNode* node = new MinusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<MinusASTNode> newNode = shared_ptr<MinusASTNode>(node);
  return newNode;
}