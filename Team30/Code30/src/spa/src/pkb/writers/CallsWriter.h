#pragma once

#include "pkb/storage/ProcedureAndCallsStorage.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ICallsWriter.h"

class CallsWriter : public ICallsWriter {
 public:
  explicit CallsWriter(CallsStorage *, ProcedureAndCallsStorage *);

  void addCalls(StmtValue stmt, EntityValue currProc,
                EntityValue calledProc) override;

 private:
  CallsStorage *callStorage;
  ProcedureAndCallsStorage *procAndCallsStorage;
};
