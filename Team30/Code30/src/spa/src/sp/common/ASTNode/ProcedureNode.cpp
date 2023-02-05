#include <memory>
#include "ProcedureNode.h"

using std::shared_ptr;

ProcedureNode::ProcedureNode() {}

void ProcedureNode::accept() {}

string ProcedureNode::toString() {
  string ss;
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
