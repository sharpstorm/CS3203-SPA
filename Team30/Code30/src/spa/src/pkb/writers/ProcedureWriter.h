#pragma once

#include <string>

#include "../../common/Types.h"
#include "../storage/ProcedureStorage.h"
class ProcedureWriter {
 public:
  explicit ProcedureWriter(ProcedureStorage* procedureStorage);
  void addProcedure(const std::string&, int, int);

 private:
  ProcedureStorage* procedureStorage;
};
