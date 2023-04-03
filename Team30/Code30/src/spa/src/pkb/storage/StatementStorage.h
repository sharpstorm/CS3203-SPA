#pragma once

#include "common/Types.h"
#include "pkb/storage/tables/IBaseSetTable.h"
#include "pkb/storage/tables/IBaseTable.h"

class StatementStorage {
 private:
  IBaseTable<StmtValue, StmtType>* table;
  IBaseSetTable<StmtType, StmtValue>* reverseTable;
  StmtSet* allStatements;

 public:
  StatementStorage(IBaseTable<StmtValue, StmtType>* table,
                   IBaseSetTable<StmtType, StmtValue>* reverseTable,
                   StmtSet* values);

  void insert(StmtValue stmt, StmtType type);

  const StmtSet& getStatementsOfType(StmtType type) const;

  const StmtSet& getAllStatements() const;

  StmtType getTypeOfStatement(StmtValue stmt) const;
};
