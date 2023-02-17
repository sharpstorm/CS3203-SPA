#include <memory>
#include "ProcedureNode.h"
#include "sp/extractor/Extractor.h"

using std::shared_ptr;

ProcedureNode::ProcedureNode(string name): procName(name),
                                           ASTNode(ASTNODE_PROCEDURE, "") {
}

void ProcedureNode::accept(shared_ptr<Extractor> e) {}

void ProcedureNode::leave(shared_ptr<Extractor> e) {}

string ProcedureNode::toString() {
  string ss = "Procedure:" + procName + " {";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  ss += "}";
  return ss;
}
