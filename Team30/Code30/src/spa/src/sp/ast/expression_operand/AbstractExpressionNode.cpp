#include "AbstractExpressionNode.h"

AbstractExpressionNode::AbstractExpressionNode(
    ASTNodeType type, const ASTExpressionSymbol &symbol):
    BinaryASTNode(type), symbolicRepr(symbol) {}

void AbstractExpressionNode::accept(IExtractor* e) const {
  e->visitExpression(this);
}

ASTExpressionSymbol AbstractExpressionNode::getValue() const {
  return symbolicRepr;
}
