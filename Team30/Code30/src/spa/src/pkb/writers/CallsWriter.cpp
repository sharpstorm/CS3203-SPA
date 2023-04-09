#include "CallsWriter.h"

CallsWriter::CallsWriter(CallsStorage* callStorage,
                         ProcedureAndCallsStorage* procAndCallsStorage)
    : callStorage(callStorage), procAndCallsStorage(procAndCallsStorage) {}

void CallsWriter::addCalls(StmtValue stmt, EntityValue currProc,
                           EntityValue calledProc) {
  callStorage->insert(currProc, calledProc);
  procAndCallsStorage->addCallDeclaration(stmt, calledProc);
}
