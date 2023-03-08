#include "ProcedureWriter.h"
#include "pkb/errors/PKBError.h"

ProcedureWriter::ProcedureWriter(ProcedureStorage *procedureStorage)
    : procedureStorage(procedureStorage) {}

void ProcedureWriter::addProcedure(string procedureName,
                                   int startLineNum, int endLineNum) {
  auto procedures = procedureStorage->getAllValues();
  if (procedures.find(procedureName) != procedures.end()) {
    throw PKBError(PKBERR_DUPLICATE_PROCEDURE);
  }

  procedureStorage->insertFromTo(startLineNum, endLineNum, procedureName);
}
