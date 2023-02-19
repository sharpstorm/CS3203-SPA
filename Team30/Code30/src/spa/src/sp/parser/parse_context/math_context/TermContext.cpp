#include <memory>
#include "TermContext.h"
#include "common/ASTNode/math/math_operand/TimesASTNode.h"
#include "common/ASTNode/math/math_operand/DivASTNode.h"
#include "common/ASTNode/math/math_operand/ModASTNode.h"

using std::shared_ptr, std::make_shared;

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

  shared_ptr<BinaryASTNode> middleNode = generateOperand(curToken);
  if (middleNode == nullptr) {
    return state->getCached();
  }
  middleNode->setLeftChild(state->getCached());

  state->advanceToken();
  state->clearCached();
  shared_ptr<ASTNode> rightTerm = contextProvider
      ->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  middleNode->setRightChild(rightTerm);
  state->setCached(middleNode);

  if (state->getCurrToken() == nullptr) {
    return middleNode;
  } else if (state->getCurrToken()
      ->isType(SIMPLE_TOKEN_TIMES, SIMPLE_TOKEN_DIV, SIMPLE_TOKEN_MOD)) {
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  }
  return middleNode;
}

shared_ptr<BinaryASTNode> TermContext::generateOperand(
    SourceToken* curToken) {
  switch (curToken->getType()) {
    case SIMPLE_TOKEN_TIMES:
      return make_shared<TimesASTNode>();
    case SIMPLE_TOKEN_DIV:
      return make_shared<DivASTNode>();
    case SIMPLE_TOKEN_MOD:
      return make_shared<ModASTNode>();
    default:
      return nullptr;
  }
}
