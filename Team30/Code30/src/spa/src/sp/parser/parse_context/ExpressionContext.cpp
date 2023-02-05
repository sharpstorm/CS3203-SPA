#include <memory>
#include "ExpressionContext.h"

using std::shared_ptr;

bool ExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ExpressionContext::generateSubtree(
    SourceParseState *state) {

  if (!state->peekNextToken()->isType(SIMPLE_TOKEN_PLUS) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MINUS) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_TIMES) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_DIV) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MOD) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_INTEGER) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_VARIABLE)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }

  if (!state->peekNextToken()->isType(SIMPLE_TOKEN_PLUS)  ||
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MINUS)) {
    return contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
  } else {
    shared_ptr<ASTNode> leftNode = contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
    SourceToken* token = expect(state, SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS);
    shared_ptr<AbstractMathASTNode> middleNode; middleNode = generatePlus(state, leftNode);
    middleNode->setRightChild(contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
    return middleNode;
  }

}

shared_ptr<PlusASTNode> ExpressionContext::generatePlus(
    SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  PlusASTNode* node = new PlusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<PlusASTNode> newNode = shared_ptr<PlusASTNode>(node);
  return newNode;
}

shared_ptr<MinusASTNode> ExpressionContext::generateMinus(
    SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  MinusASTNode* node = new MinusASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<MinusASTNode> newNode = shared_ptr<MinusASTNode>(node);
  return newNode;
}
