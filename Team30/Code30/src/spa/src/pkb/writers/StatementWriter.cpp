#include "StatementWriter.h"

StatementWriter::StatementWriter(StatementStorage *statementStorage)
    : statementStorage(statementStorage) {}

void StatementWriter::addStatement(StmtValue lineNumber, StmtType stmtType) {
  statementStorage->insert(lineNumber, stmtType);
}
