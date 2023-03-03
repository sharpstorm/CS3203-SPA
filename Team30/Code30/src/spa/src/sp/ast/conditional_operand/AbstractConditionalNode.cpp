#include "AbstractConditionalNode.h"

void AbstractConditionalNode::accept(Extractor *e) {
  e->visit(this);
}

string AbstractConditionalNode::toString() {
  return identifier;
}
