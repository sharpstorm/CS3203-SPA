#include "AbstractExpressionNode.h"

AbstractExpressionNode::AbstractExpressionNode(ASTNodeType type,
                                               string symbol):
    BinaryASTNode(type), symbolicRepr(symbol) {}

void AbstractExpressionNode::accept(IExtractor* e) {
  e->visitExpression(this);
}

string AbstractExpressionNode::getValue() {
  return symbolicRepr;
}
