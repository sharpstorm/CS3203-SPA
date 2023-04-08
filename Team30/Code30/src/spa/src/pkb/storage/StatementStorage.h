#pragma once

#include "common/Types.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/IBaseSetTable.h"
#include "pkb/storage/tables/IBaseTable.h"

class StatementStorage {
 private:
  ContiguousTable<StmtType>* table;
  HashKeySetTable<StmtType, StmtValue>* reverseTable;
  StmtValueSet* allStatements;

 public:
  StatementStorage(ContiguousTable<StmtType>* table,
                   HashKeySetTable<StmtType, StmtValue>* reverseTable,
                   StmtValueSet* values);

  void insert(StmtValue stmt, StmtType type);

  const StmtValueSet& getStatementsOfType(StmtType type) const;

  const StmtValueSet& getAllStatements() const;

  StmtType getTypeOfStatement(StmtValue stmt) const;
};
