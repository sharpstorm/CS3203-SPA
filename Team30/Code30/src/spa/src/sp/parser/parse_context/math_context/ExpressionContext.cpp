#include <memory>
#include "ExpressionContext.h"

using std::shared_ptr;

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

  shared_ptr<BinaryASTNode> middleNode;
  if (curToken->isType(SIMPLE_TOKEN_PLUS) || curToken->isType(SIMPLE_TOKEN_MINUS)) {
    middleNode = generateOperand(state, state->getCached());
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

shared_ptr<BinaryASTNode> ExpressionContext::generateOperand(
    SourceParseState* state, shared_ptr<ASTNode> leftNode) {
  shared_ptr<BinaryASTNode> node;
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_PLUS:
      node = shared_ptr<BinaryASTNode>(new PlusASTNode());
      break;
    case SIMPLE_TOKEN_MINUS:
      node = shared_ptr<BinaryASTNode>(new MinusASTNode());
      break;
  }
  node->setLeftChild(leftNode);
  return node;
}
