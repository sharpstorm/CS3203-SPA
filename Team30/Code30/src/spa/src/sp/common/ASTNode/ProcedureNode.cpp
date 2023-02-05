#include <memory>
#include "ProcedureNode.h"

using std::shared_ptr;

ProcedureNode::ProcedureNode(string name): procName(name) {}

void ProcedureNode::accept() {}

string ProcedureNode::toString() {
  string ss = "Procedure(" + procName + ") {";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  ss += "}";
  return ss;
}
