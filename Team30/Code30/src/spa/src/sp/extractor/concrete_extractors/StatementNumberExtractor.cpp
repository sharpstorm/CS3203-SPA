#include "StatementNumberExtractor.h"

void StatementNumberExtractor::visit(IfNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visit(WhileNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visit(ReadNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visit(PrintNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visit(AssignNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visit(CallNode *node) {
  statementNumber = node->getLineNumber();
}

int StatementNumberExtractor::getStatementNumber() {
  return statementNumber;
}
