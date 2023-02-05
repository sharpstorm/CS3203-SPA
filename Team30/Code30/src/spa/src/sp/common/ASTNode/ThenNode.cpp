#include <memory>
#include "ThenNode.h"

using std::shared_ptr;

ThenNode::ThenNode() {}

void ThenNode::accept() {}

string ThenNode::toString() {
  string ss = "T";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
