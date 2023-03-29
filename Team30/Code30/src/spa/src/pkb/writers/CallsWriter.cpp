#include "CallsWriter.h"

CallsWriter::CallsWriter(CallsStorage* callStorage,
                         ProcedureAndCallsStorage* procAndCallsStorage)
    : callStorage(callStorage), procAndCallsStorage(procAndCallsStorage) {}

void CallsWriter::addCalls(int stmt, string currProc, string calledProc) {
  callStorage->insert(currProc, calledProc);
  procAndCallsStorage->addCallDeclaration(stmt, calledProc);
}
