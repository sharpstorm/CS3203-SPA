#include "ProcedureNode.h"
#include "sp/extractor/IExtractor.h"

ProcedureNode::ProcedureNode(const ProcedureName &name) :
    procName(name), ASTNode(ASTNODE_PROCEDURE) {
}

ProcedureName ProcedureNode::getName() const {
  return procName;
}

void ProcedureNode::accept(IExtractor *e) const {
  e->visitProcedure(this);
}

void ProcedureNode::leave(IExtractor *e) const {
  e->leaveProcedure(this);
}

ASTNode *ProcedureNode::getChildStatement(
    const ProcedureStmtIndex &statementIndex) const {
  return children.at(0)->getMutableChild(statementIndex);
}
