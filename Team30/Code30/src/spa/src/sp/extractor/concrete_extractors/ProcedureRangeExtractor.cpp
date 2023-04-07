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
  const ASTNode *stmtList = node->getMutableChild(0);
  handleProcedure(node->getName(), stmtList);
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
  const ASTNode *stmtList = node->getMutableChild(2);
  recurseContainer(stmtList);
}

void ProcedureRangeExtractor::visitWhile(const WhileNode *node) {
  const ASTNode *stmtList = node->getMutableChild(1);
  recurseContainer(stmtList);
}

void ProcedureRangeExtractor::handleProcedure(const ProcedureName &name,
                                              const ASTNode *stmtList) {
  StatementNumberExtractor statementNoExtractor;
  ASTNodeRefList statements = stmtList->getChildren();
  statements.front()->accept(&statementNoExtractor);
  statements.back()->accept(this);
  addProcedureRange(name,
                    statementNoExtractor.getStatementNumber(),
                    lineNumberCache);
}

void ProcedureRangeExtractor::recurseContainer(const ASTNode *stmtList) {
  stmtList->getChildren().back()->accept(this);
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
