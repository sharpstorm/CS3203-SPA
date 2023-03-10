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

string DesignEntitiesQueryHandler::getVariableByIndex(int index) const {
  return entityMappingProvider->getVariableByIndex(index);
}

string DesignEntitiesQueryHandler::getConstantByIndex(int index) const {
  return entityMappingProvider->getConstantByIndex(index);
}

unordered_set<int> DesignEntitiesQueryHandler::getIndexOfVariable(
    string name) const {
  return entityMappingProvider->getIndexOfVariable(name);
}

unordered_set<int> DesignEntitiesQueryHandler::getIndexOfConstant(
    string name) const {
  return entityMappingProvider->getIndexOfConstant(name);
}
