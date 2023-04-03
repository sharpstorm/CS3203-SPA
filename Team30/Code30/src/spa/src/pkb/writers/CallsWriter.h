#pragma once

#include <string>

#include "pkb/storage/ProcedureAndCallsStorage.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ICallsWriter.h"

using std::string;

class CallsWriter : public ICallsWriter {
 public:
  explicit CallsWriter(CallsStorage *, ProcedureAndCallsStorage *);

  void addCalls(int stmt, string currProc, string calledProc) override;

 private:
  CallsStorage *callStorage;
  ProcedureAndCallsStorage *procAndCallsStorage;
};
