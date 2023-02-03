#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../../common/Types.h"

class StatementStorage {
 public:
    StatementStorage();
    void addStatement(int, StmtType);
    StmtType getStatement(int);
    std::vector<int> getStatementsOfType(StmtType);
 private:
    std::vector<StmtType> StatementTable;
    std::unordered_map<StmtType, std::vector<int>> ReverseStatementTable;
};
