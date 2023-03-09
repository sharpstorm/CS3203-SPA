#include <memory>
#include <utility>
#include "ExpressionContext.h"

#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/expression_operand/MinusASTNode.h"

using std::make_unique;

ExpressionContext::ExpressionContext(IExpressionContextProvider *provider):
    RecursiveParseContext(provider) {}

ASTNodePtr ExpressionContext::generateSubtree(SourceParseState *state) {
  ASTNodePtr leftExpr;
  if (!state->hasCached()) {
    // Expect term
    leftExpr = contextProvider
        ->generateSubtree(ExpressionContextType::TERM_CONTEXT, state);
  } else {
    leftExpr = state->consumeCache();
  }

  BinaryASTNodePtr middleNode = generateOperand(state);
  if (middleNode == nullptr) {
    return std::move(leftExpr);
  }
  middleNode->setLeftChild(std::move(leftExpr));

  ASTNodePtr rightTerm = contextProvider
      ->generateSubtree(ExpressionContextType::TERM_CONTEXT, state);
  middleNode->setRightChild(std::move(rightTerm));

  if (state->currTokenIsOfType(SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS)) {
    state->cacheNode(std::move(middleNode));
    return contextProvider
        ->generateSubtree(ExpressionContextType::EXPR_CONTEXT, state);
  }
  return std::move(middleNode);
}

BinaryASTNodePtr ExpressionContext::generateOperand(SourceParseState *state) {
  SourceToken* curToken = state->tryExpect(SIMPLE_TOKEN_PLUS,
                                           SIMPLE_TOKEN_MINUS);
  if (curToken == nullptr) {
    return nullptr;
  }

  switch (curToken->getType()) {
    case SIMPLE_TOKEN_PLUS:
      return make_unique<PlusASTNode>();
    case SIMPLE_TOKEN_MINUS:
      return make_unique<MinusASTNode>();
    default:
      return nullptr;
  }
}
