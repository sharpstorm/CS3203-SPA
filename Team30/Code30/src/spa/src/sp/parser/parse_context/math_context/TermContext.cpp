#include <memory>
#include "TermContext.h"
#include "common/ASTNode/math/math_operand/TimesASTNode.h"
#include "common/ASTNode/math/math_operand/DivASTNode.h"
#include "common/ASTNode/math/math_operand/ModASTNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> TermContext::generateSubtree(SourceParseState *state) {
  if (!state->hasCached()) {
    // Expect term
    shared_ptr<ASTNode> firstExpr = contextProvider
        ->getContext(FACTOR_CONTEXT)->generateSubtree(state);
    state->setCached(firstExpr);
  }

  SourceToken* curToken = state->getCurrToken();
  if (curToken == nullptr) {
    return state->getCached();
  }

  shared_ptr<BinaryASTNode> middleNode;
  if (curToken->isType(SIMPLE_TOKEN_TIMES) ||
      curToken->isType(SIMPLE_TOKEN_DIV) ||
      curToken->isType(SIMPLE_TOKEN_MOD)) {
    middleNode = generateOperand(state, state->getCached());
  } else {
    return state->getCached();
  }

  state->advanceToken();
  state->clearCached();
  shared_ptr<ASTNode> rightTerm = contextProvider
      ->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  middleNode->setRightChild(rightTerm);
  state->setCached(middleNode);

  if (state->getCurrToken() == nullptr) {
    return middleNode;
  } else if (state->getCurrToken()->isType(SIMPLE_TOKEN_TIMES)
      || state->getCurrToken()->isType(SIMPLE_TOKEN_DIV)
      || state->getCurrToken()->isType(SIMPLE_TOKEN_MOD)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }
  return middleNode;
}

shared_ptr<BinaryASTNode> TermContext::generateOperand(
    SourceParseState* state, shared_ptr<ASTNode> leftNode) {
  shared_ptr<BinaryASTNode> node;
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_TIMES:
      node = shared_ptr<BinaryASTNode>(new TimesASTNode());
      break;
    case SIMPLE_TOKEN_DIV:
      node = shared_ptr<BinaryASTNode>(new DivASTNode());
      break;
    case SIMPLE_TOKEN_MOD:
      node = shared_ptr<BinaryASTNode>(new ModASTNode());
      break;
  }
  node->setLeftChild(leftNode);
  return node;
}
