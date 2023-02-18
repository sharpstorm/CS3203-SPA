#include "StatementWriter.h"

StatementWriter::StatementWriter(StatementStorage *statementStorage)
    : statementStorage(statementStorage) {}

void StatementWriter::addStatement(int lineNumber, StmtType stmtType) {
  statementStorage->insert(lineNumber, stmtType);
}
