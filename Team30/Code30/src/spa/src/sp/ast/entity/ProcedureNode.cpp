#include "ProcedureNode.h"
#include "sp/extractor/IExtractor.h"

ProcedureNode::ProcedureNode(string name): procName(name),
                                           ASTNode(ASTNODE_PROCEDURE) {
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

ASTNode *ProcedureNode::getChildStatement(const int &statementIndex) {
  return children.at(0)->getChildren()[statementIndex];
}
