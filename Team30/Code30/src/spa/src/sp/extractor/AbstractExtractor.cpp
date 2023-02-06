#include "AbstractExtractor.h"
#include "sp/common/ASTNode/statement/IfNode.h"
#include "sp/common/ASTNode/statement/ReadNode.h"
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/common/ASTNode/Statement/PrintNode.h"
#include "sp/common/ASTNode/Statement/AssignNode.h"
#include "sp/common/ASTNode/Statement/WhileNode.h"

void AbstractExtractor::visit(IfNode node) {
}

void AbstractExtractor::visit(ReadNode node) {
}

void AbstractExtractor::visit(StatementListNode node) {
}

void AbstractExtractor::visit(AssignNode node) {
}

void AbstractExtractor::visit(PrintNode node) {
}

void AbstractExtractor::visit(WhileNode node) {
}