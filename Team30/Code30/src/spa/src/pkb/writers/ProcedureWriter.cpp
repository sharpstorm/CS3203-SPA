#include "ProcedureWriter.h"

ProcedureWriter::ProcedureWriter(ProcedureStorage *procedureStorage) {}

void ProcedureWriter::addProcedure \
    (const std::string & procedureName, int startLineNum, int endLineNum) {
    procedureStorage->addProcedure(procedureName, startLineNum, endLineNum);
}

