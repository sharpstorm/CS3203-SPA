#include "AbstractExtractor.h"

void AbstractExtractor::visit(IfNode node) {}

void AbstractExtractor::visit(ReadNode node) {}

void AbstractExtractor::visit(StatementListNode node) {}

void AbstractExtractor::visit(AssignNode node) {}

void AbstractExtractor::visit(PrintNode node) {}

void AbstractExtractor::visit(WhileNode node) {}

void AbstractExtractor::visit(VariableASTNode node) {}

void AbstractExtractor::visit(ConstantASTNode node) {}

void AbstractExtractor::leave(IfNode node) {}

void AbstractExtractor::leave(WhileNode node) {}
