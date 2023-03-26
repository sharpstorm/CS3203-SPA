#pragma once

#include "common/Types.h"
#include "pkb/storage/tables/IBaseSetTable.h"
#include "pkb/storage/tables/IBaseTable.h"

class StatementStorage {
 private:
  IBaseTable<StmtValue, StmtType>* table;
  IBaseSetTable<StmtType, StmtValue>* reverseTable;
  StmtValueSet* allStatements;

 public:
  StatementStorage(IBaseTable<StmtValue, StmtType>* table,
                   IBaseSetTable<StmtType, StmtValue>* reverseTable,
                   StmtValueSet* values)
      : table(table), reverseTable(reverseTable), allStatements(values){};

  void insert(StmtValue stmt, StmtType type);

  StmtValueSet getStatementsOfType(StmtType type) const;

  StmtValueSet getAllStatements() const;

  StmtType getTypeOfStatement(StmtValue stmt) const;
};
