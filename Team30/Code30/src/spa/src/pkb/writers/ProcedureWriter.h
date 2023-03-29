#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/storage/ProcedureAndCallsStorage.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"

using std::string;

class ProcedureWriter : public IProcedureWriter {
 public:
  explicit ProcedureWriter(ProcedureAndCallsStorage *procedureStorage);
  void addProcedure(string, int, int) override;

 private:
  ProcedureAndCallsStorage *procedureStorage;
};
