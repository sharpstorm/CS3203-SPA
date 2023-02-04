#pragma once

#include <vector>
#include <string>
#include <utility>
#include "StatementStorage.h"
#include "ProcedureStorage.h"
#include "../../common/Types.h"

class StructureMappingProvider {
 public:
    StructureMappingProvider();
    static StatementStorage* statementStorage;
    static ProcedureStorage* procedureStorage;
    StmtType getStatementType(int);
    bool isStatementOfType(int, StmtType);
    std::vector<int> getStatementsOfType(StmtType);
    std::pair<int, int> getProcedureLines(std::string);
    std::string getProcedureForLine(int);
};
