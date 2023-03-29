#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/HashKeyTable.h"

using std::string;
using std::unordered_map;
using std::unordered_set;

class StructureMappingProviderStub : public IStructureMappingProvider {
 public:
  HashKeySetTable<StmtType, int> stmtTypeToNum;
  HashKeyTable<int, StmtType> stmtNumToType;
  unordered_set<int> allStmts;

  HashKeyTable<int, string> stmtNumToProcedure;

  HashKeyTable<int, string> stmtNumToCalledProcedure;

  StructureMappingProviderStub();

  StmtType getStatementType(int stmt) const override;

  unordered_set<int> getValuesOfType(StmtType stmtType) const override;

  bool isValueOfType(StmtType, StmtValue) const override;

  string getProcedureForLine(int stmt) const override;

  string getCalledDeclaration(int) const override;
};
