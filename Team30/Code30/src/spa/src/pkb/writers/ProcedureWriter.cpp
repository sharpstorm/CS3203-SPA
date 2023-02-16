#include "ProcedureWriter.h"

ProcedureWriter::ProcedureWriter(ProcedureStorage *procedureStorage)
    : procedureStorage(procedureStorage) {}

void ProcedureWriter::addProcedure(string procedureName,
                                   int startLineNum, int endLineNum) {
  procedureStorage->insertFromTo(startLineNum, endLineNum, procedureName);
}
