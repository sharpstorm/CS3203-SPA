#pragma once

#include <unordered_set>
#include <string>
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/ContiguousTable.h"

using std::unordered_set;
using std::string;

class EntityMappingProviderStub : public IEntityMappingProvider {
 public:
  unordered_set<string> allVariables;
  unordered_set<string> allConstants;
  unordered_set<string> allProcedures;

  ContiguousTable<string> variableTable;
  ContiguousTable<string> constantTable;
  ContiguousTable<string> procedureTable;

  HashKeySetTable<string, int> variableToIndexTable;
  HashKeySetTable<string, int> constantToIndexTable;
  HashKeySetTable<string, int> procedureToStmtNumTable;

  EntityMappingProviderStub();
  unordered_set<string> getSymbolsOfType(EntityType) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  unordered_set<int> getIndexOfVariable(string) const override;
  unordered_set<int> getIndexOfConstant(string) const override;
  bool isSymbolOfType(EntityType, string) const override;
};
