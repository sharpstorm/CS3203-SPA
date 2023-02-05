#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "../../common/Types.h"

class ProcedureStorage {
 public:
  ProcedureStorage();
  void addProcedure(const std::string &, int, int);
  std::pair<int, int> getProcedure(const std::string &) const;
  std::string getProcedureForStatement(int) const;
 private:
  std::unordered_map<std::string, std::pair<int, int> > ProcedureTable;
  std::vector<std::string> ReverseProcedureTable;
};
