#include "ProcedureRangeExtractor.h"
#include "StatementNumberExtractor.h"
#include "pkb/errors/PKBError.h"
#include "sp/errors/SPError.h"

ProcedureRangeExtractor::ProcedureRangeExtractor(PkbWriter *writer)
    : pkbWriter(writer), lineNumberCache(0) {
}

void ProcedureRangeExtractor::updateCache(const StatementASTNode *node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitProcedure(const ProcedureNode *node) {
  StatementNumberExtractor statementNoExtractor;
  ASTNodeRefList statements = node->getMutableChild(0)->getChildren();
  statements.front()->accept(&statementNoExtractor);
  statements.back()->accept(this);
  addProcedureRange(node->getName(),
                    statementNoExtractor.getStatementNumber(),
                    lineNumberCache);
}

void ProcedureRangeExtractor::visitRead(const ReadNode *node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitPrint(const PrintNode *node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitAssign(const AssignNode *node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitCall(const CallNode *node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitIf(const IfNode *node) {
  node->getMutableChild(2)->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::visitWhile(const WhileNode *node) {
  node->getMutableChild(1)->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::
addProcedureRange(const ProcedureName &procName,
                  LineNumber start,
                  LineNumber end) {
  try {
    pkbWriter->addProcedure(procName, start, end);
  } catch (PKBError e) {
    throw SPError(e.what());
  }
}
