#include <memory>
#include "TermContext.h"
#include "../../common/ASTNode/math/PlusASTNode.h"
#include "sp/common/ASTNode/math/DivASTNode.h"
#include "sp/common/ASTNode/math/ModASTNode.h"

using std::shared_ptr;

bool TermContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> TermContext::generateSubtree(SourceParseState *state) {

  if (!state->peekNextToken()->isType(SIMPLE_TOKEN_PLUS) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MINUS) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_TIMES) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_DIV) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MOD) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_INTEGER) &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_VARIABLE)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }

  if (!state->peekNextToken()->isType(SIMPLE_TOKEN_TIMES)  &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_DIV)  &&
      !state->peekNextToken()->isType(SIMPLE_TOKEN_MOD)) {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  } else {
    shared_ptr<ASTNode> leftNode = contextProvider->getContext(TERM_CONTEXT)->generateSubtree(state);
    SourceToken* token = expect(state, SIMPLE_TOKEN_TIMES, SIMPLE_TOKEN_DIV, SIMPLE_TOKEN_MOD);
    shared_ptr<AbstractMathASTNode> middleNode; middleNode = generateTimes(state, leftNode);
    middleNode->setRightChild(contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state));
    return middleNode;
  }
}

shared_ptr<AbstractMathASTNode>
TermContext::generateTimes(SourceParseState *state,
                           shared_ptr<ASTNode> leftNode) {
  TimesASTNode* node = new TimesASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<TimesASTNode> newNode = shared_ptr<TimesASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode>
TermContext::generateDiv(SourceParseState *state,
                         shared_ptr<ASTNode> leftNode) {
  DivASTNode* node = new DivASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<DivASTNode> newNode = shared_ptr<DivASTNode>(node);
  return newNode;
}

shared_ptr<AbstractMathASTNode>
TermContext::generateMod(SourceParseState *state,
                         shared_ptr<ASTNode> leftNode) {
  ModASTNode* node = new ModASTNode();
  node->setLeftChild(leftNode);
  shared_ptr<ModASTNode> newNode = shared_ptr<ModASTNode>(node);
  return newNode;
}
