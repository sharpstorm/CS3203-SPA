#include "ProcedureStorage.h"

ProcedureStorage::ProcedureStorage()
    : ProcedureTable(), ReverseProcedureTable() {}

void ProcedureStorage::addProcedure(const std::string &procedureName,
                                    int startLineNum, int endLineNum) {
  ProcedureTable[procedureName] = std::make_pair(startLineNum, endLineNum);
  if (endLineNum > ReverseProcedureTable.size()) {
    ReverseProcedureTable.resize(endLineNum * 2);
  }
  for (int i = startLineNum; i < endLineNum + 1; ++i) {
    ReverseProcedureTable[i] = procedureName;
  }
}

std::pair<int, int> ProcedureStorage::getProcedure(
    const std::string &procedureName) const {
  return ProcedureTable.at(procedureName);
}

std::string ProcedureStorage::getProcedureForStatement(int lineNumber) const {
  return ReverseProcedureTable.at(lineNumber);
}
