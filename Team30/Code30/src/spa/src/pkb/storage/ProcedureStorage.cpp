#include "ProcedureStorage.h"

ProcedureStorage::ProcedureStorage() {}

void ProcedureStorage::addProcedure \
    (const std::string & procedureName, int startLineNum, int endLineNum) {
    ProcedureTable[procedureName] = std::make_pair(startLineNum, endLineNum);
    ReverseProcedureTable.insert \
        (ReverseProcedureTable.begin() + startLineNum, procedureName);
    ReverseProcedureTable.insert \
        (ReverseProcedureTable.begin() + endLineNum, procedureName);
}

std::pair<int, int> ProcedureStorage::getProcedure \
    (const std::string& procedureName) {
    return ProcedureTable.at(procedureName);
}

std::string ProcedureStorage::getProcedureForStatement(int lineNumber) {
    return ReverseProcedureTable.at(lineNumber);
}

