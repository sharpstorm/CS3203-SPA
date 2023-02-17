#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/HashKeyTable.h"
#include "pkb/PkbTypes.h"

using std::unordered_map;
using std::unordered_set;
using std::string;

class StructureMappingProviderStub : public IStructureMappingProvider {
 public:
  HashKeySetTable<StmtType, int> stmtTypeToNum;
  HashKeyTable<int, StmtType> stmtNumToType;
  unordered_set<int> allStmts;

  HashKeySetTable<string, int> procedureToStmtNum;
  HashKeyTable<int, string> stmtNumToProcedure;

  StructureMappingProviderStub();

  StmtType getStatementType(int stmt) const override;

  unordered_set<int> getStatementsOfType(StmtType stmtType) const override;

  unordered_set<int> getProcedureLines(string procedure) const override;

  string getProcedureForLine(int stmt) const override;

  Transformer<int, string> getStmtProcedureTransformer() const override;
};
