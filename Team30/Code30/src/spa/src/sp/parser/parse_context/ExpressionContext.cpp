#include <memory>
#include "ExpressionContext.h"

using std::shared_ptr;

bool ExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ExpressionContext::generateSubtree(
    SourceParseState *state) {

  if (!state->hasCached()) {
    // Expect term
    shared_ptr<ASTNode> firstExpr = contextProvider
        ->getContext(TERM_CONTEXT)->generateSubtree(state);
    state->setCached(firstExpr);
  }

  SourceToken* curToken = state->getCurrToken();
  if (curToken == nullptr) {
    return state->getCached();
  }

  shared_ptr<AbstractMathASTNode> middleNode;
  if (curToken->isType(SIMPLE_TOKEN_PLUS)) {
    middleNode = generatePlus(state->getCached());
  } else if (curToken->isType(SIMPLE_TOKEN_MINUS)) {
    middleNode = generateMinus(state->getCached());
  } else {
    return state->getCached();
  }

  state->advanceToken();
  state->clearCached();

  shared_ptr<ASTNode> rightTerm = contextProvider
      ->getContext(TERM_CONTEXT)->generateSubtree(state);
  middleNode->setRightChild(rightTerm);
  state->setCached(middleNode);

  if (state->getCurrToken() == nullptr) {
    return middleNode;
  } else if (state->getCurrToken()->isType(SIMPLE_TOKEN_PLUS)
    || state->getCurrToken()->isType(SIMPLE_TOKEN_MINUS)) {
    return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }
  return middleNode;
}

shared_ptr<PlusASTNode> ExpressionContext::generatePlus(
    shared_ptr<ASTNode> leftNode) {
  PlusASTNode* node = new PlusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<PlusASTNode> newNode = shared_ptr<PlusASTNode>(node);
  return newNode;
}

shared_ptr<MinusASTNode> ExpressionContext::generateMinus(
    shared_ptr<ASTNode> leftNode) {
  MinusASTNode* node = new MinusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<MinusASTNode> newNode = shared_ptr<MinusASTNode>(node);
  return newNode;
}
