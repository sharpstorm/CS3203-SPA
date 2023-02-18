#include "AbstractMathNode.h"

void AbstractMathNode::accept(shared_ptr<Extractor> e) {
  e->visit(this);
}

string AbstractMathNode::toString() {
  return identifier;
}
