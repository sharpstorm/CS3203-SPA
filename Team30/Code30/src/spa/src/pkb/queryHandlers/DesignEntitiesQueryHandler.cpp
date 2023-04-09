#include "DesignEntitiesQueryHandler.h"

DesignEntitiesQueryHandler::DesignEntitiesQueryHandler(
    const IEntityMappingProvider *entityMappingProvider,
    const IStructureMappingProvider *structureMappingProvider)
    : entityMappingProvider(entityMappingProvider),
      structureMappingProvider(structureMappingProvider) {}

EntityValueSet DesignEntitiesQueryHandler::getSymbolsOfType(
    EntityType entityType) const {
  return entityMappingProvider->getValuesOfType(entityType);
}

StmtValueSet DesignEntitiesQueryHandler::getStatementsOfType(
    StmtType stmtType) const {
  return structureMappingProvider->getValuesOfType(stmtType);
}

StmtType DesignEntitiesQueryHandler::getStatementType(StmtValue stmtNo) const {
  return structureMappingProvider->getStatementType(stmtNo);
}

EntityValue DesignEntitiesQueryHandler::getVariableByIndex(
    EntityIdx index) const {
  return entityMappingProvider->getVariableByIndex(index);
}

EntityValue DesignEntitiesQueryHandler::getConstantByIndex(
    EntityIdx index) const {
  return entityMappingProvider->getConstantByIndex(index);
}

EntityIdx DesignEntitiesQueryHandler::getIndexOfVariable(
    EntityValue name) const {
  return entityMappingProvider->getIndexOfVariable(name);
}

EntityIdx DesignEntitiesQueryHandler::getIndexOfConstant(
    EntityValue name) const {
  return entityMappingProvider->getIndexOfConstant(name);
}

bool DesignEntitiesQueryHandler::isStatementOfType(StmtType stmtType,
                                                   StmtValue stmtNo) const {
  return structureMappingProvider->isValueOfType(stmtType, stmtNo);
}

bool DesignEntitiesQueryHandler::isSymbolOfType(EntityType entityType,
                                                EntityValue name) const {
  return entityMappingProvider->isValueOfType(entityType, name);
}

EntityValue DesignEntitiesQueryHandler::getCalledDeclaration(
    StmtValue callStmt) const {
  return structureMappingProvider->getCalledDeclaration(callStmt);
}
