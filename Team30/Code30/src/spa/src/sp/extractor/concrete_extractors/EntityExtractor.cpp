#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void EntityExtractor::visitProcedure(ProcedureNode *node) {
  pkbWriter->addSymbol(node->getName(), EntityType::Procedure);
  procNameCache = node->getName();
}

void EntityExtractor::visitPrint(PrintNode* node) {
  addStatement<StmtType::Print>(node);
}

void EntityExtractor::visitAssign(AssignNode* node) {
  addStatement<StmtType::Assign>(node);
}

void EntityExtractor::visitWhile(WhileNode* node) {
  addStatement<StmtType::While>(node);
}

void EntityExtractor::visitIf(IfNode* node) {
  addStatement<StmtType::If>(node);
}

void EntityExtractor::visitRead(ReadNode* node) {
  addStatement<StmtType::Read>(node);
}

void EntityExtractor::visitCall(CallNode* node) {
  pkbWriter->addCalls(node->getLineNumber(), procNameCache, node->getName());
}

void EntityExtractor::visitVariable(VariableASTNode* node) {
  pkbWriter->addSymbol(node->getValue(), EntityType::Variable);
}

void EntityExtractor::visitConstant(ConstantASTNode* node) {
  pkbWriter->addSymbol(node->getValue(), EntityType::Constant);
}

