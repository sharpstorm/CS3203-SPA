#pragma once

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/HashKeyTable.h"

class StructureMappingProviderStub : public IStructureMappingProvider {
 public:
  HashKeySetTable<StmtType, StmtValue> stmtTypeToNum;
  HashKeyTable<StmtValue, StmtType> stmtNumToType;
  StmtValueSet allStmts;

  HashKeyTable<StmtValue, EntityValue> stmtNumToProcedure;

  HashKeyTable<StmtValue, EntityValue> stmtNumToCalledProcedure;

  StructureMappingProviderStub();

  void insert(StmtValue stmt, StmtType);

  StmtType getStatementType(StmtValue stmt) const override;

  const StmtValueSet& getValuesOfType(StmtType stmtType) const override;

  bool isValueOfType(StmtType, StmtValue) const override;

  EntityValue getProcedureForLine(StmtValue stmt) const override;

  EntityValue getCalledDeclaration(StmtValue) const override;
};
