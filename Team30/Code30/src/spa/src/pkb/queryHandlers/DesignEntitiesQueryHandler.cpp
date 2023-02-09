#include "DesignEntitiesQueryHandler.h"

DesignEntitiesQueryHandler::DesignEntitiesQueryHandler(
    const IEntityMappingProvider *entityMappingProvider,
    const IStructureMappingProvider *structureMappingProvider)
    : entityMappingProvider(entityMappingProvider),
      structureMappingProvider(structureMappingProvider) {}

std::unordered_set<std::string> DesignEntitiesQueryHandler::getSymbolsOfType(
    EntityType entityType) const {
  return entityMappingProvider->getSymbolsOfType(entityType);
}

std::unordered_set<int> DesignEntitiesQueryHandler::getStatementsOfType(
    StmtType stmtType) const {
  return structureMappingProvider->getStatementsOfType(stmtType);
}
