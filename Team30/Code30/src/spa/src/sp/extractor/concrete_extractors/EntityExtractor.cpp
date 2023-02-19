#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void EntityExtractor::visit(ProcedureNode *node) {
  pkbWriter->addSymbol(node->getName(), EntityType::Procedure);
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

void EntityExtractor::visit(VariableASTNode* node) {
  pkbWriter->addSymbol(node->getValue(), EntityType::Variable);
}

void EntityExtractor::visit(ConstantASTNode* node) {
  pkbWriter->addSymbol(node->getValue(), EntityType::Constant);
}
