#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void EntityExtractor::visit(ProcedureNode *node) {
//  pkbWriter->addProcedure(node->getName(), );
  procNameCache = node->getName();
}

void EntityExtractor::visit(PrintNode* node) {
  pkbWriter->addStatement(node->getLineNumber(), StmtType::Print);
}

void EntityExtractor::visit(AssignNode* node) {
  pkbWriter->addStatement(node->getLineNumber(), StmtType::Assign);
}

void EntityExtractor::visit(WhileNode* node) {
  pkbWriter->addStatement(node->getLineNumber(), StmtType::While);
}

void EntityExtractor::visit(IfNode* node) {
  pkbWriter->addStatement(node->getLineNumber(), StmtType::If);
}

void EntityExtractor::visit(ReadNode* node) {
  pkbWriter->addStatement(node->getLineNumber(), StmtType::Read);
}

void EntityExtractor::visit(CallNode* node) {
  pkbWriter->addCalls(node->getLineNumber(), procNameCache, node->getName());
}

void EntityExtractor::visit(VariableASTNode* node) {
  pkbWriter->addVariable(node->getValue());
}

void EntityExtractor::visit(ConstantASTNode* node) {
  pkbWriter->addConstant(node->getValue());
}

