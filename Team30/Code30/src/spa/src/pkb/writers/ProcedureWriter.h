#pragma once

#include <string>
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"

using std::string;

class ProcedureWriter : public IProcedureWriter {
 public:
  explicit ProcedureWriter(ProcedureStorage *procedureStorage);
  void addProcedure(string, int, int) override;

 private:
  ProcedureStorage *procedureStorage;
};
