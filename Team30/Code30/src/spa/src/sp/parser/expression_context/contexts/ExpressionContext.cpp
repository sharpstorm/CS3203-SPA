#include <memory>
#include "ExpressionContext.h"

#include "common/ast/math/math_operand/PlusASTNode.h"
#include "common/ast/math/math_operand/MinusASTNode.h"

using std::make_shared;

ASTNodePtr ExpressionContext::generateSubtree(
    SourceParseState *state) {

  if (!state->hasCached()) {
    // Expect term
    shared_ptr<ASTNode> firstExpr = contextProvider
        ->getContext(ExpressionContextType::TERM_CONTEXT)
        ->generateSubtree(state);
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
      ->getContext(ExpressionContextType::TERM_CONTEXT)
      ->generateSubtree(state);
  middleNode->setRightChild(rightTerm);
  state->setCached(middleNode);

  if (state->getCurrToken() == nullptr) {
    return middleNode;
  } else if (state->getCurrToken()
      ->isType(SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS)) {
    return contextProvider->getContext(ExpressionContextType::EXPR_CONTEXT)
    ->generateSubtree(state);
  }
  return middleNode;
}

BinaryASTNodePtr ExpressionContext::generateOperand(
    SourceToken* curToken) {
  switch (curToken->getType()) {
    case SIMPLE_TOKEN_PLUS:
      return make_shared<PlusASTNode>();
    case SIMPLE_TOKEN_MINUS:
      return make_shared<MinusASTNode>();
    default:
      return nullptr;
  }
}
