#include "AbstractExpressionNode.h"

void AbstractExpressionNode::accept(Extractor* e) {
  e->visit(this);
}

string AbstractExpressionNode::toString() {
  return identifier;
}
