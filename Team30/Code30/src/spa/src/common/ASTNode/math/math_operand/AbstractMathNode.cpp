#include "AbstractMathNode.h"

void AbstractMathNode::accept(Extractor* e) {
  e->visit(this);
}

string AbstractMathNode::toString() {
  return identifier;
}
