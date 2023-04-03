#include "ProcedureRangeExtractor.h"
#include "StatementNumberExtractor.h"
#include "pkb/errors/PKBError.h"
#include "sp/errors/SPError.h"

ProcedureRangeExtractor::ProcedureRangeExtractor(PkbWriter* writer)
    : pkbWriter(writer), lineNumberCache(0) {
}

void ProcedureRangeExtractor::updateCache(StatementASTNode *node) {
  lineNumberCache = node->getLineNumber();
}

void ProcedureRangeExtractor::visitProcedure(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;
  vector<ASTNode*> statements = node->getChildren()[0]->getChildren();
  statements.front()->accept(&statementNoExtractor);
  statements.back()->accept(this);
  addProcedureRange(node->getName(),
                    statementNoExtractor.getStatementNumber(),
                    lineNumberCache);
}

void ProcedureRangeExtractor::visitRead(ReadNode* node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitPrint(PrintNode* node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitAssign(AssignNode* node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitCall(CallNode* node) {
  updateCache(node);
}

void ProcedureRangeExtractor::visitIf(IfNode* node) {
  node->getChildren()[2]->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::visitWhile(WhileNode* node) {
  node->getChildren()[1]->getChildren().back()->accept(this);
}

void ProcedureRangeExtractor::
addProcedureRange(const string &procName, LineNumber start, LineNumber end) {
  try {
    pkbWriter->addProcedure(procName, start, end);
  } catch (PKBError e) {
    throw SPError(e.what());
  }
}
