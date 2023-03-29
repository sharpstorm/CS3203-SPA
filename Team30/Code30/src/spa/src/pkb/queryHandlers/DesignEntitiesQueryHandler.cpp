#include "DesignEntitiesQueryHandler.h"

DesignEntitiesQueryHandler::DesignEntitiesQueryHandler(
    const IEntityMappingProvider *entityMappingProvider,
    const IStructureMappingProvider *structureMappingProvider)
    : entityMappingProvider(entityMappingProvider),
      structureMappingProvider(structureMappingProvider) {}

unordered_set<string> DesignEntitiesQueryHandler::getSymbolsOfType(
    EntityType entityType) const {
  EntitySet entitySet = entityMappingProvider->getValuesOfType(entityType);
  unordered_set<string> result(entitySet.begin(), entitySet.end());
  return result;
}

unordered_set<int> DesignEntitiesQueryHandler::getStatementsOfType(
    StmtType stmtType) const {
  StmtSet stmtSet = structureMappingProvider->getValuesOfType(stmtType);
  unordered_set<int> result(stmtSet.begin(), stmtSet.end());
  return result;
}

StmtType DesignEntitiesQueryHandler::getStatementType(int stmtNo) const {
  return structureMappingProvider->getStatementType(stmtNo);
}

string DesignEntitiesQueryHandler::getVariableByIndex(int index) const {
  return entityMappingProvider->getVariableByIndex(index);
}

string DesignEntitiesQueryHandler::getConstantByIndex(int index) const {
  return entityMappingProvider->getConstantByIndex(index);
}

EntityIdx DesignEntitiesQueryHandler::getIndexOfVariable(string name) const {
  return entityMappingProvider->getIndexOfVariable(name);
}

EntityIdx DesignEntitiesQueryHandler::getIndexOfConstant(string name) const {
  return entityMappingProvider->getIndexOfConstant(name);
}

bool DesignEntitiesQueryHandler::isStatementOfType(StmtType stmtType,
                                                   int stmtNo) const {
  return structureMappingProvider->isValueOfType(stmtType, stmtNo);
}

bool DesignEntitiesQueryHandler::isSymbolOfType(EntityType entityType,
                                                string name) const {
  return entityMappingProvider->isValueOfType(entityType, name);
}

string DesignEntitiesQueryHandler::getCalledDeclaration(int callStmt) const {
  return structureMappingProvider->getCalledDeclaration(callStmt);
}
