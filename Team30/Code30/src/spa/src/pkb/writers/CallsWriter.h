#pragma once

#include <string>

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ICallsWriter.h"

using std::string;

class CallsWriter : public ICallsWriter {
 public:
  explicit CallsWriter(CallsStorage *callsStore,
                       CallStmtStorage *callStmtStore);

  void addCalls(int arg1, string arg2, string arg3) override;

 private:
  CallsStorage *callStorage;
  CallStmtStorage *callStmtStorage;
};
