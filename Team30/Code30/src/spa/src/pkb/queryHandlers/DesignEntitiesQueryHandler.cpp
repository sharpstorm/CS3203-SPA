#include "DesignEntitiesQueryHandler.h"

DesignEntitiesQueryHandler::DesignEntitiesQueryHandler(
    const IEntityMappingProvider *entityMappingProvider,
    const IStructureMappingProvider *structureMappingProvider)
    : entityMappingProvider(entityMappingProvider),
      structureMappingProvider(structureMappingProvider) {}

std::unordered_set<std::string> DesignEntitiesQueryHandler::getValuesOfType(
    EntityType entityType) const {
  return entityMappingProvider->getValuesOfType(entityType);
}

std::unordered_set<int> DesignEntitiesQueryHandler::getValuesOfType(
    StmtType stmtType) const {
  return structureMappingProvider->getValuesOfType(stmtType);
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

unordered_set<int> DesignEntitiesQueryHandler::getIndexOfVariable(
    string name) const {
  return entityMappingProvider->getIndexOfVariable(name);
}

unordered_set<int> DesignEntitiesQueryHandler::getIndexOfConstant(
    string name) const {
  return entityMappingProvider->getIndexOfConstant(name);
}

bool DesignEntitiesQueryHandler::isStatementOfType(StmtType stmtType,
                                                   int stmtNo) const {
  return structureMappingProvider->isStatementOfType(stmtType, stmtNo);
}

bool DesignEntitiesQueryHandler::isSymbolOfType(EntityType entityType,
                                                string name) const {
  return entityMappingProvider->isSymbolOfType(entityType, name);
}

string DesignEntitiesQueryHandler::getCalledDeclaration(int callStmt) const {
  return structureMappingProvider->getCalledProcedure(callStmt);
}
