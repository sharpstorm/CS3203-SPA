#include <string>
#include "ProcedureRangeExtractor.h"
#include "StatementNumberExtractor.h"

ProcedureRangeExtractor::ProcedureRangeExtractor(PkbWriter* writer)
    : pkbWriter(writer) {
}

void ProcedureRangeExtractor::visit(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;
  ASTNodePtr stmtLst = node->getChildren()[0];
  stmtLst->getChildren().front()->accept(&statementNoExtractor);
  int start = statementNoExtractor.getStatementNumber();
  stmtLst->getChildren().back()->accept(this);
  addProcedureRange(node->getName(), start, lineNumberCache);
}

void ProcedureRangeExtractor::visit(ReadNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visit(PrintNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visit(AssignNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visit(CallNode* node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visit(IfNode* node) {
  node->getChildren()[2]->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::visit(WhileNode* node) {
  node->getChildren()[1]->getChildren().back()->accept(this);
}


void ProcedureRangeExtractor::
addProcedureRange(string procName, int start, int end) {
  pkbWriter->addProcedure(procName, start, end);
}
