#include "StatementNumberExtractor.h"

void StatementNumberExtractor::updateLineNumber(StatementASTNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitIf(IfNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitWhile(WhileNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitRead(ReadNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitPrint(PrintNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitAssign(AssignNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitCall(CallNode *node) {
  updateLineNumber(node);
}

const int StatementNumberExtractor::getStatementNumber() {
  return statementNumber;
}
