#include <memory>
#include "TermContext.h"
#include "common/ASTNode/math/TimesASTNode.h"
#include "common/ASTNode/math/DivASTNode.h"
#include "common/ASTNode/math/ModASTNode.h"

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

  shared_ptr<AbstractMathASTNode> middleNode;
  if (curToken->isType(SIMPLE_TOKEN_TIMES)) {
    middleNode = generateTimes(state->getCached());
  } else if (curToken->isType(SIMPLE_TOKEN_DIV)) {
    middleNode = generateDiv(state->getCached());
  } else if (curToken->isType(SIMPLE_TOKEN_MOD)) {
    middleNode = generateMod(state->getCached());
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
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  }
  return middleNode;
}

shared_ptr<AbstractMathASTNode>
TermContext::generateTimes(shared_ptr<ASTNode> leftNode) {
  TimesASTNode* node = new TimesASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<TimesASTNode> newNode = shared_ptr<TimesASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode>
TermContext::generateDiv(shared_ptr<ASTNode> leftNode) {
  DivASTNode* node = new DivASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<DivASTNode> newNode = shared_ptr<DivASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode>
TermContext::generateMod(shared_ptr<ASTNode> leftNode) {
  ModASTNode* node = new ModASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<ModASTNode> newNode = shared_ptr<ModASTNode>(node);
  return newNode;
}
