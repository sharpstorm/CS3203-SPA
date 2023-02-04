#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider() {}

StmtType StructureMappingProvider::getStatementType(int lineNumber) {
    return statementStorage->getStatement(lineNumber);
}

bool StructureMappingProvider::isStatementOfType \
    (int lineNumber, StmtType stmtType) {
    return stmtType == statementStorage->getStatement(lineNumber);
}

std::vector<int> StructureMappingProvider::getStatementsOfType \
    (StmtType stmtType) {
    return statementStorage->getStatementsOfType(stmtType);
}

std::pair<int, int> StructureMappingProvider::getProcedureLines \
    (std::string procedureName) {
    return procedureStorage->getProcedure(procedureName);
}

std::string StructureMappingProvider::getProcedureForLine(int lineNumber) {
    return procedureStorage->getProcedureForStatement(lineNumber);
}
