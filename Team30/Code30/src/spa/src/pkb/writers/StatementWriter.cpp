#include "StatementWriter.h"

StatementWriter::StatementWriter(StatementStorage *statementStorage)
    : statementStorage(statementStorage) {}

void StatementWriter::addStatement(int lineNumber, StmtType stmtType) {
  statementStorage->addStatement(lineNumber, stmtType);
}

void StatementWriter::addContainerStmt(int start, int end) {}
