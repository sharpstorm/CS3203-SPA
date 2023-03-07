#include <memory>
#include "ExpressionContext.h"

#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/expression_operand/MinusASTNode.h"

using std::make_shared;

ExpressionContext::ExpressionContext(IExpressionContextProvider *provider):
    RecursiveParseContext(provider) {}

ASTNodePtr ExpressionContext::generateSubtree(
    SourceParseState *state) {

  if (!state->hasCached()) {
    // Expect term
    shared_ptr<ASTNode> firstExpr = contextProvider
        ->generateSubtree(ExpressionContextType::TERM_CONTEXT, state);
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
      ->generateSubtree(ExpressionContextType::TERM_CONTEXT, state);
  middleNode->setRightChild(rightTerm);
  state->setCached(middleNode);

  curToken = state->getCurrToken();
  if (curToken == nullptr) {
    return middleNode;
  } else if (curToken->isType(SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS)) {
    return contextProvider
        ->generateSubtree(ExpressionContextType::EXPR_CONTEXT, state);
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
