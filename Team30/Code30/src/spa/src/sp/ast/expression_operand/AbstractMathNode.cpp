#include "AbstractExpressionNode.h"

AbstractExpressionNode::AbstractExpressionNode(ASTNodeType type,
                                               string identifier,
                                               string symbol):
    BinaryASTNode(type), identifier(identifier), symbolicRepr(symbol) {}

void AbstractExpressionNode::accept(Extractor* e) {
  e->visitExpression(this);
}

string AbstractExpressionNode::toString() {
  return identifier;
}

string AbstractExpressionNode::getValue() {
  return symbolicRepr;
}
