#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../common/Types.h"

class StatementStorage {
 public:
  StatementStorage();
  void addStatement(int, StmtType);
  StmtType getStatement(int) const;
  std::unordered_set<int> getStatementsOfType(StmtType) const;

 private:
  std::vector<StmtType> StatementTable;
  std::unordered_map<StmtType, std::unordered_set<int>> ReverseStatementTable;
};
