#include <memory>
#include <utility>
#include "TermContext.h"

#include "sp/ast/expression_operand/TimesASTNode.h"
#include "sp/ast/expression_operand/DivASTNode.h"
#include "sp/ast/expression_operand/ModASTNode.h"

using std::make_unique;

ASTNodePtr TermContext::generateSubtree(SourceParseState *state) {
  ASTNodePtr leftExpr;
  if (!state->hasCached()) {
    leftExpr = contextProvider
        ->generateSubtree(ExpressionContextType::FACTOR_CONTEXT, state);
  } else {
    leftExpr = state->consumeCache();
  }

  BinaryASTNodePtr middleNode = generateOperand(state);
  if (middleNode == nullptr) {
    return leftExpr;
  }
  middleNode->setLeftChild(std::move(leftExpr));

  ASTNodePtr rightTerm = contextProvider
      ->generateSubtree(ExpressionContextType::FACTOR_CONTEXT, state);
  middleNode->setRightChild(std::move(rightTerm));

  if (state->currTokenIsOfType(SIMPLE_TOKEN_TIMES,
                               SIMPLE_TOKEN_DIV,
                               SIMPLE_TOKEN_MOD)) {
    state->cacheNode(std::move(middleNode));
    return contextProvider
        ->generateSubtree(ExpressionContextType::TERM_CONTEXT, state);
  }

  return middleNode;
}

BinaryASTNodePtr TermContext::generateOperand(SourceParseState *state) {
  SourceToken* curToken = state->tryExpect(SIMPLE_TOKEN_TIMES,
                                           SIMPLE_TOKEN_DIV,
                                           SIMPLE_TOKEN_MOD);
  if (curToken == nullptr) {
    return nullptr;
  }

  switch (curToken->getType()) {
    case SIMPLE_TOKEN_TIMES:
      return make_unique<TimesASTNode>();
    case SIMPLE_TOKEN_DIV:
      return make_unique<DivASTNode>();
    case SIMPLE_TOKEN_MOD:
      return make_unique<ModASTNode>();
    default:
      return nullptr;
  }
}
