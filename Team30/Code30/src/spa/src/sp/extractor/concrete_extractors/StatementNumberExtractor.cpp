#include "StatementNumberExtractor.h"

void StatementNumberExtractor::visitIf(IfNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitWhile(WhileNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitRead(ReadNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitPrint(PrintNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitAssign(AssignNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitCall(CallNode *node) {
  statementNumber = node->getLineNumber();
}

int StatementNumberExtractor::getStatementNumber() {
  return statementNumber;
}
