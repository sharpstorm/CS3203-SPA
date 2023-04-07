#include "EntityExtractor.h"
#include "sp/errors/SPError.h"

EntityExtractor::EntityExtractor(PkbWriter *writer) : pkbWriter(writer) {}

void EntityExtractor::visitProcedure(const ProcedureNode *node) {
  procNameCache = node->getName();
}

void EntityExtractor::visitPrint(const PrintNode *node) {
  addStatement<StmtType::Print>(node);
}

void EntityExtractor::visitAssign(const AssignNode *node) {
  addStatement<StmtType::Assign>(node);
}

void EntityExtractor::visitWhile(const WhileNode *node) {
  addStatement<StmtType::While>(node);
}

void EntityExtractor::visitIf(const IfNode *node) {
  addStatement<StmtType::If>(node);
}

void EntityExtractor::visitRead(const ReadNode *node) {
  addStatement<StmtType::Read>(node);
}

void EntityExtractor::visitCall(const CallNode *node) {
  if (procNameCache == node->getName()) {
    throw SPError(SPERR_PROCEDURE_SELF_CALL);
  }
  addStatement<StmtType::Call>(node);
  pkbWriter->addCalls(node->getLineNumber(), procNameCache, node->getName());
}

void EntityExtractor::visitVariable(const VariableASTNode *node) {
  pkbWriter->addVariable(node->getValue());
}

void EntityExtractor::visitConstant(const ConstantASTNode *node) {
  pkbWriter->addConstant(node->getValue());
}
