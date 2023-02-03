//
// Created by Ddx on 3/2/23.
//
#include "StatementWriter.h"

StatementWriter::StatementWriter(StatementStorage *statementStorage) {

}

void StatementWriter::addStatement(int lineNumber, StmtType stmtType) {
    statementStorage->addStatement(lineNumber, stmtType);
}

