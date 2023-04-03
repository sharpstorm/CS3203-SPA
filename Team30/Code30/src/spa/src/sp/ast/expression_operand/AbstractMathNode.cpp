#include "AbstractExpressionNode.h"

AbstractExpressionNode::AbstractExpressionNode(ASTNodeType type):
    BinaryASTNode(type) {}

void AbstractExpressionNode::accept(IExtractor* e) {
  e->visitExpression(this);
}

string AbstractExpressionNode::getValue() {
  return symbolicRepr;
}
