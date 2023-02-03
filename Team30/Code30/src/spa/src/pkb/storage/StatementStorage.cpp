//
// Created by Ddx on 4/2/23.
//
#include "StatementStorage.h"

StatementStorage::StatementStorage() {

}

void StatementStorage::addStatement(int lineNumber, StmtType stmtType) {
    StatementTable.insert(StatementTable.begin() + lineNumber, stmtType);
    // add to reverse table
    ReverseStatementTable[stmtType].push_back(lineNumber);
}

StmtType StatementStorage::getStatement(int lineNumber) {
    return StatementTable.at(lineNumber);
}

std::vector<int> StatementStorage::getStatementsOfType(StmtType stmtType) {
    return ReverseStatementTable.at(stmtType);
}

