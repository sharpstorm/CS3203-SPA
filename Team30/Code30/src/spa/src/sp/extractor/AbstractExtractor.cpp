#include "AbstractExtractor.h"


void AbstractExtractor::visit(IfNode node) {}

void AbstractExtractor::visit(ReadNode node) {}

void AbstractExtractor::visit(StatementListNode node) {}

void AbstractExtractor::visit(AssignNode node) {}

void AbstractExtractor::visit(PrintNode node) {}

void AbstractExtractor::visit(WhileNode node) {}

void AbstractExtractor::visit(CallNode node) {}

void AbstractExtractor::visit(VariableASTNode node) {}

void AbstractExtractor::visit(ConstantASTNode node) {}

void AbstractExtractor::visit(AbstractMathNode node) {}

void AbstractExtractor::visit(AbstractConditionalNode node) {}

void AbstractExtractor::leave(IfNode node) {}

void AbstractExtractor::leave(ReadNode node) {}

void AbstractExtractor::leave(StatementListNode node) {}

void AbstractExtractor::leave(AssignNode node) {}

void AbstractExtractor::leave(PrintNode node) {}

void AbstractExtractor::leave(WhileNode node) {}

void AbstractExtractor::leave(CallNode node) {}

void AbstractExtractor::leave(VariableASTNode node) {}

void AbstractExtractor::leave(ConstantASTNode node) {}

void AbstractExtractor::leave(AbstractMathNode node) {}

void AbstractExtractor::leave(AbstractConditionalNode node) {}
