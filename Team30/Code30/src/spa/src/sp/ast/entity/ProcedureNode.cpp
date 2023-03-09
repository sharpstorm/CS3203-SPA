#include <memory>
#include "ProcedureNode.h"
#include "sp/extractor/IExtractor.h"

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

void ProcedureNode::accept(IExtractor* e) {
  e->visitProcedure(this);
}

void ProcedureNode::leave(IExtractor* e) {
  e->leaveProcedure(this);
}
