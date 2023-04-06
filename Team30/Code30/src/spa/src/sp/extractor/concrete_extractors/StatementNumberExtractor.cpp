#include "StatementNumberExtractor.h"

void StatementNumberExtractor::updateLineNumber(const StatementASTNode *node) {
  statementNumber = node->getLineNumber();
}

void StatementNumberExtractor::visitIf(const IfNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitWhile(const WhileNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitRead(const ReadNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitPrint(const PrintNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitAssign(const AssignNode *node) {
  updateLineNumber(node);
}

void StatementNumberExtractor::visitCall(const CallNode *node) {
  updateLineNumber(node);
}

const LineNumber StatementNumberExtractor::getStatementNumber() {
  return statementNumber;
}
