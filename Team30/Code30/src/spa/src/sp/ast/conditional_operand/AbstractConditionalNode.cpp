#include "AbstractConditionalNode.h"

void AbstractConditionalNode::accept(IExtractor *e) {
  e->visitConditional(this);
}
