#include "CFGsQueryHandler.h"

#include <string>

using std::string;

CFGsQueryHandler::CFGsQueryHandler(
    const CFGStorage *cfgsStorage,
    const IEntityMappingProvider *entityProvider,
    const IStructureMappingProvider *structureProvider)
    : cfgsStorage(cfgsStorage), entityProvider(entityProvider),
    structureProvider(structureProvider) {}

vector<CFG*> CFGsQueryHandler::queryCFGs(StmtRef stmt) const {
  vector<CFG*> result;
  if (!stmt.isKnown()) {
    unordered_set<string> procedures = entityProvider
        ->getSymbolsOfType(EntityType::Procedure);
    for (auto it = procedures.begin(); it != procedures.end(); it++) {
      result.push_back(cfgsStorage->get(*it).get());
    }
  } else {
    string procedureName = structureProvider->getProcedureForLine(stmt.lineNum);
    result.push_back(cfgsStorage->get(procedureName).get());
  }

  return result;
}
