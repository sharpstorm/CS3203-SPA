#pragma once

#include "common/Types.h"
#include "pkb/storage/ProcedureAndCallsStorage.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"

class ProcedureWriter : public IProcedureWriter {
 public:
  explicit ProcedureWriter(ProcedureAndCallsStorage *procedureStorage);
  void addProcedure(EntityValue, StmtValue, StmtValue) override;

 private:
  ProcedureAndCallsStorage *procedureStorage;
};
