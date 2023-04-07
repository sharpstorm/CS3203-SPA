#include "AbstractConditionalNode.h"

void AbstractConditionalNode::accept(IExtractor *e) const {
  e->visitConditional(this);
}
