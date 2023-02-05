#include <memory>
#include "ThenNode.h"

using std::shared_ptr;

string ThenNode::toString() {
  string ss = "";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
