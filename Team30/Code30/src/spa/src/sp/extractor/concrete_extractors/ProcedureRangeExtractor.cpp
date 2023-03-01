#include "ProcedureRangeExtractor.h"
#include "StatementNumberExtractor.h"

ProcedureRangeExtractor::ProcedureRangeExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ProcedureRangeExtractor::visit(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;
  node->getChildren()[0]->getChildren().front()->accept(&statementNoExtractor);
  int start = statementNoExtractor.getStatementNumber();
  node->getChildren()[0]->getChildren().back()->accept(&statementNoExtractor);
  int end = statementNoExtractor.getStatementNumber();
  addProcedureRange(node->getName(), start, end);
}

void ProcedureRangeExtractor::addProcedureRange(string procName, int start, int end) {
  pkbWriter->addProcedure(procName, start, end);
}
