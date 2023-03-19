#pragma once

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/OverrideTable.h"

class QueryExecutorAgent {
 public:
  QueryExecutorAgent(PkbQueryHandler* pkb, OverrideTable* table);
  StmtRef transformArg(const PQLSynonymName name, const StmtRef &ref) const;
  EntityRef transformArg(const PQLSynonymName name, const EntityRef &ref) const;
  bool isValid(const StmtRef &ref) const;
  bool isValid(const EntityRef &ref) const;

  PkbQueryHandler* operator->() const;

 private:
  PkbQueryHandler* pkbQueryHandler;
  OverrideTable* overrideTable;

  static bool isValidRef(StmtRef stmtRef, PkbQueryHandler* handler);
  static bool isValidRef(EntityRef entRef, PkbQueryHandler* handler);
};
