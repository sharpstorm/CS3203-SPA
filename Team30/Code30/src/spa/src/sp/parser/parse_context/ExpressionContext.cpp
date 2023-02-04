#include "ExpressionContext.h"

#include "../../common/ASTNode/math/PlusASTNode.h"


#include <iostream>
bool ExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> ExpressionContext::generateSubtree(SourceParseState *state) {
  if (!state->hasCached()) {
    // I'm first
    state->setCached(contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
  }

  shared_ptr<ASTNode> leftNode = state->getCached();
  shared_ptr<PlusASTNode> middleNode;
  SourceToken* token = expect(state, SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS);
  if (token->isType(SIMPLE_TOKEN_PLUS)) {
    middleNode = generatePlus(state, leftNode);
    middleNode->setRightChild(contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state));
    state->setCached(middleNode);
  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_MINUS)) {

  }

  if (state->peekNextToken()->isType(SIMPLE_TOKEN_PLUS) ||
      state->peekNextToken()->isType(SIMPLE_TOKEN_MINUS)) {
    return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }

  return middleNode;
//  shared_ptr<ASTNode> leftNode = contextProvider->getContext(VARIABLE_CONTEXT)->generateSubtree(state);
//  SourceToken* token = expect(state, SIMPLE_TOKEN_PLUS, SIMPLE_TOKEN_MINUS);
//
//  switch (token->getType()) {
//    case SIMPLE_TOKEN_PLUS:
//      return generatePlus(state, leftNode);
//    case SIMPLE_TOKEN_MINUS:
//      break;
//    default:
//      throw SPError("Unknown token sequence");
//  }
}

shared_ptr<PlusASTNode> ExpressionContext::generatePlus(SourceParseState *state, shared_ptr<ASTNode> leftNode) {
  PlusASTNode* node = new PlusASTNode();
  node->setLeftChild(leftNode);

  shared_ptr<PlusASTNode> newNode = shared_ptr<PlusASTNode>(node);
  return newNode;
}
