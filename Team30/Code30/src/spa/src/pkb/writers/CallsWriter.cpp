#include "CallsWriter.h"

CallsWriter::CallsWriter(CallsStorage *callsStore,
                         CallStmtStorage *callStmtStore)
    : callStorage(callsStore), callStmtStorage(callStmtStore) {}

void CallsWriter::addCalls(int arg1, string arg2, string arg3) {
  callStorage->insert(arg2, arg3);
  callStmtStorage->insert(arg1, arg3);
}
