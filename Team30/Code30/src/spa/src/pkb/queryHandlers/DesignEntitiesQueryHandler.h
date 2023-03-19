#pragma once

#include <string>
#include <unordered_set>

#include "common/Types.h"
#include "pkb/storage/EntityMappingProvider.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/queryHandlers/interfaces/IDesignEntitiesQueryHandler.h"

using std::string;
using std::unordered_set;

class DesignEntitiesQueryHandler : public IDesignEntitiesQueryHandler {
 public:
  DesignEntitiesQueryHandler(
      const IEntityMappingProvider *entityMappingProvider,
      const IStructureMappingProvider *structureMappingProvider);

  unordered_set<string> getSymbolsOfType(EntityType) const override;
  unordered_set<int> getStatementsOfType(StmtType) const override;
  StmtType getStatementType(int) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  unordered_set<int> getIndexOfVariable(string) const override;
  unordered_set<int> getIndexOfConstant(string) const override;
  bool isStatementOfType(StmtType, int) const override;
  bool isSymbolOfType(EntityType, string) const override;
  string getCalledDeclaration(int) const override;

 private:
  const IEntityMappingProvider *entityMappingProvider;
  const IStructureMappingProvider *structureMappingProvider;
};
