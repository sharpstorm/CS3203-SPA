#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void EntityExtractor::visit(PrintNode node) {
  pkbWriter->addStatement(node.lineNumber, StmtType::Print);
}

void EntityExtractor::visit(AssignNode node) {
  pkbWriter->addStatement(node.lineNumber, StmtType::Assign);
}

void EntityExtractor::visit(WhileNode node) {
  pkbWriter->addStatement(node.lineNumber, StmtType::While);
}

void EntityExtractor::visit(IfNode node) {
  pkbWriter->addStatement(node.lineNumber, StmtType::If);
}

void EntityExtractor::visit(ReadNode node) {
  pkbWriter->addStatement(node.lineNumber, StmtType::Read);
}

void EntityExtractor::visit(VariableASTNode node) {
  pkbWriter->addSymbol(node.getValue(), EntityType::Variable);
}

void EntityExtractor::visit(ConstantASTNode node) {
  pkbWriter->addSymbol(node.getValue(), EntityType::Constant);
}
