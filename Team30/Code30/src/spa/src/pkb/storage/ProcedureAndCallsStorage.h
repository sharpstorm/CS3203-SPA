#pragma once

#include "StorageTypes.h"
#include "common/Types.h"
#include "pkb/errors/PKBError.h"
#include "pkb/storage/tables/IBaseSetTable.h"
#include "pkb/storage/tables/IBaseTable.h"

class ProcedureAndCallsStorage {
 private:
  ProcedureStmtTable* procedureStmtTable;
  CallDeclarationTable* callDeclarationTable;
  ProcedureValues* procedureValues;
  ProcedureValues undefinedProcedures;

 public:
  ProcedureAndCallsStorage(ProcedureStmtTable* procedureStmtTable,
                           CallDeclarationTable* callDeclarationTable,
                           ProcedureValues* procedureValues);
  bool procedureExists(EntityValue procedure) const;

  bool hasUndefinedProcedures() const;

  void addProcedure(StmtValue start, StmtValue end, EntityValue procedure);

  void addCallDeclaration(StmtValue stmt, EntityValue calledProcedure);

  EntityValue getProcedureForLine(StmtValue lineNumber) const;

  EntityValue getCalledDeclaration(StmtValue stmt) const;

  EntityValueSet getProcedures() const;

 private:
  void insertFromTo(StmtValue startNum, StmtValue endNum,
                    EntityValue procedure);
};
