#include "ProcedureRangeExtractor.h"
#include "StatementNumberExtractor.h"

ProcedureRangeExtractor::ProcedureRangeExtractor(PkbWriter* writer)
    : pkbWriter(writer), lineNumberCache(0) {
}

void ProcedureRangeExtractor::visitProcedure(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;
  ASTNode* stmtLst = node->getChildren()[0];
  stmtLst->getChildren().front()->accept(&statementNoExtractor);
  int start = statementNoExtractor.getStatementNumber();
  stmtLst->getChildren().back()->accept(this);
  addProcedureRange(node->getName(), start, lineNumberCache);
}

void ProcedureRangeExtractor::visitRead(ReadNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitPrint(PrintNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitAssign(AssignNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitCall(CallNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitIf(IfNode* node) {
  node->getChildren()[2]->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::visitWhile(WhileNode* node) {
  node->getChildren()[1]->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::
addProcedureRange(const string &procName, int start, int end) {
  pkbWriter->addProcedure(procName, start, end);
}
