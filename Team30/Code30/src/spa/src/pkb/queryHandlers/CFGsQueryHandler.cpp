#include "CFGsQueryHandler.h"

CFGsQueryHandler::CFGsQueryHandler(
    const CFGStorage *cfgsStorage, const IEntityMappingProvider *entityProvider,
    const IStructureMappingProvider *structureProvider)
    : cfgsStorage(cfgsStorage),
      entityProvider(entityProvider),
      structureProvider(structureProvider) {}

vector<CFG *> CFGsQueryHandler::queryCFGs(StmtRef stmt) const {
  vector<CFG *> result;
  if (!stmt.isKnown()) {
    EntityValueSet procedures =
        entityProvider->getValuesOfType(EntityType::Procedure);
    for (auto it = procedures.begin(); it != procedures.end(); it++) {
      result.push_back(cfgsStorage->get(*it).get());
    }
  } else {
    EntityValue procedureName =
        structureProvider->getProcedureForLine(stmt.getValue());
    if (procedureName.empty()) {
      return result;
    }
    result.push_back(cfgsStorage->get(procedureName).get());
  }

  return result;
}
