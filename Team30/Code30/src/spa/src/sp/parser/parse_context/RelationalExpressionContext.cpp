#include "RelationalExpressionContext.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"
#include <map>


std::map<SourceTokenType, string> relationalCharacterMap{
        {SIMPLE_TOKEN_GT, ">"},
        {SIMPLE_TOKEN_GTE, ">="},
        {SIMPLE_TOKEN_LT, "<"},
        {SIMPLE_TOKEN_LTE, "<="},
        {SIMPLE_TOKEN_EQUALS, "=="},
        {SIMPLE_TOKEN_NOT_EQUALS, "!="}
};

bool RelationalExpressionContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> RelationalExpressionContext::generateSubtree(SourceParseState *state) {
  shared_ptr<ASTNode> leftNode= contextProvider->getContext(REL_FACTOR_CONTEXT)->generateSubtree(state);
  SourceToken* token = expect(state, SIMPLE_TOKEN_GT, SIMPLE_TOKEN_GTE,
                              SIMPLE_TOKEN_LT, SIMPLE_TOKEN_LTE,
                              SIMPLE_TOKEN_EQUALS, SIMPLE_TOKEN_NOT_EQUALS);
  state->clearCached();
  shared_ptr<RelationalExpressionASTNode> newNode = generateRelationalNode(token->getValue(),
                                                                           state, leftNode);
  newNode->setRightChild(contextProvider->getContext(REL_FACTOR_CONTEXT)->generateSubtree(state));
  return newNode;
}

shared_ptr<RelationalExpressionASTNode> RelationalExpressionContext::generateRelationalNode
(string repr, SourceParseState* state, shared_ptr<ASTNode> leftNode) {
  RelationalExpressionASTNode* node = new RelationalExpressionASTNode(repr);
  shared_ptr<RelationalExpressionASTNode> newNode = shared_ptr<RelationalExpressionASTNode>(node);
  node->setLeftChild(leftNode);



  return newNode;
}