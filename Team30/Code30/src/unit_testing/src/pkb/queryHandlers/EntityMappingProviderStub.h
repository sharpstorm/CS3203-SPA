#pragma once

#include <string>
#include <set>

#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::string;
using std::set;

class EntityMappingProviderStub : public IEntityMappingProvider {
 public:
  EntitySet allVariables;
  EntitySet allConstants;
  EntitySet allProcedures;

  ContiguousTable<string> variableTable;
  ContiguousTable<string> constantTable;
  ContiguousTable<string> procedureTable;

  HashKeyTable<string, int> variableToIndexTable;
  HashKeyTable<string, int> constantToIndexTable;
  HashKeyTable<string, int> procedureToStmtNumTable;

  EntityMappingProviderStub();
  const EntitySet & getValuesOfType(EntityType) const override;
  bool isValueOfType(EntityType, EntityValue) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  EntityIdx getIndexOfVariable(string) const override;
  EntityIdx getIndexOfConstant(string) const override;
};
