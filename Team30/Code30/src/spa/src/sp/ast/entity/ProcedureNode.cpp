#include <memory>
#include "ProcedureNode.h"
#include "sp/extractor/Extractor.h"

using std::shared_ptr;

ProcedureNode::ProcedureNode(string name): procName(name),
                                           ASTNode(ASTNODE_PROCEDURE) {
}

string ProcedureNode::toString() {
  string ss = "Procedure:" + procName + " {";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  ss += "}";
  return ss;
}

string ProcedureNode::getName() {
  return procName;
}

void ProcedureNode::accept(Extractor* e) {
  e->visitProcedure(this);
}

void ProcedureNode::leave(Extractor* e) {
  e->leaveProcedure(this);
}
