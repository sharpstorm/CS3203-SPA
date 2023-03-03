#include "AbstractExpressionNode.h"

AbstractExpressionNode::AbstractExpressionNode(ASTNodeType type,
                                               string identifier,
                                               string symbol):
    BinaryASTNode(type), identifier(), symbolicRepr(symbol) {}

void AbstractExpressionNode::accept(Extractor* e) {
  e->visit(this);
}

string AbstractExpressionNode::toString() {
  return identifier;
}

string AbstractExpressionNode::getValue() {
  return symbolicRepr;
}
