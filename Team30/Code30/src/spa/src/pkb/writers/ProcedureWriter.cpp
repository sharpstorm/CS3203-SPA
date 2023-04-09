#include "ProcedureWriter.h"

#include "pkb/errors/PKBError.h"

ProcedureWriter::ProcedureWriter(ProcedureAndCallsStorage *procedureStorage)
    : procedureStorage(procedureStorage) {}

void ProcedureWriter::addProcedure(EntityValue procedureName,
                                   StmtValue startLineNum,
                                   StmtValue endLineNum) {
  if (procedureStorage->procedureExists(procedureName)) {
    throw PKBError(PKBERR_DUPLICATE_PROCEDURE);
  }
  procedureStorage->addProcedure(startLineNum, endLineNum, procedureName);
}
