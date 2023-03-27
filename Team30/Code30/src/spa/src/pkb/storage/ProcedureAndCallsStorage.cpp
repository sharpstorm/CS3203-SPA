#include "ProcedureAndCallsStorage.h"

ProcedureAndCallsStorage::ProcedureAndCallsStorage(
    ProcedureStmtTable *procedureStmtTable,
    CallDeclarationTable *callDeclarationTable,
    ProcedureValues *procedureValues)
    : procedureStmtTable(procedureStmtTable),
      callDeclarationTable(callDeclarationTable),
      procedureValues(procedureValues) {}

bool ProcedureAndCallsStorage::procedureExists(EntityValue procedure) const {
  if (procedureValues->find(procedure) == procedureValues->end()) {
    return false;
  }
  return true;
}

bool ProcedureAndCallsStorage::hasUndefinedProcedures() const {
  return !undefinedProcedures.empty();
}

void ProcedureAndCallsStorage::addProcedure(StmtValue start, StmtValue end,
                                            EntityValue procedure) {
  insertFromTo(start, end, procedure);
  procedureValues->insert(procedure);
  auto it = undefinedProcedures.find(procedure);
  if (it != undefinedProcedures.end()) {
    undefinedProcedures.erase(it);
  }
}

void ProcedureAndCallsStorage::insertFromTo(StmtValue startNum,
                                            StmtValue endNum,
                                            EntityValue procedure) {
  for (int i = startNum; i < endNum + 1; ++i) {
    procedureStmtTable->set(i, procedure);
  }
}

void ProcedureAndCallsStorage::addCallDeclaration(StmtValue stmt,
                                                  EntityValue calledProcedure) {
  callDeclarationTable->set(stmt, calledProcedure);
  if (!procedureExists(calledProcedure)) {
    undefinedProcedures.insert(calledProcedure);
  }
}

EntityValue ProcedureAndCallsStorage::getProcedureForLine(
    StmtValue lineNumber) const {
  return procedureStmtTable->get(lineNumber);
}

EntityValue ProcedureAndCallsStorage::getCalledDeclaration(
    StmtValue stmt) const {
  return callDeclarationTable->get(stmt);
}

EntityValueSet ProcedureAndCallsStorage::getProcedures() const {
  return *procedureValues;
}
