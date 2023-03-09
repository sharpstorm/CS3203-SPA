#include "AbstractConditionalNode.h"

void AbstractConditionalNode::accept(Extractor *e) {
  e->visitConditional(this);
}

string AbstractConditionalNode::toString() {
  return identifier;
}
