//
// Created by Ddx on 4/2/23.
//
#include "StatementStorage.h"

StatementStorage::StatementStorage()
    : StatementTable(), ReverseStatementTable() {}

void StatementStorage::addStatement(int lineNumber, StmtType stmtType) {
  if (lineNumber > StatementTable.size()) {
    StatementTable.resize(lineNumber * 2);
  }
  StatementTable[lineNumber] = stmtType;
  // add to reverse table
  ReverseStatementTable[stmtType].insert(lineNumber);
}

StmtType StatementStorage::getStatement(int lineNumber) const {
  return StatementTable.at(lineNumber);
}

std::unordered_set<int> StatementStorage::getStatementsOfType(
    StmtType stmtType) const {
  return ReverseStatementTable.at(stmtType);
}
