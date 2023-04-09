#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(
    VariableStorage *variableStorage, ConstantStorage *constantStorage,
    ProcedureAndCallsStorage *procedureStorage)
    : variableStorage(variableStorage),
      constantStorage(constantStorage),
      procedureStorage(procedureStorage) {}

const EntityValueSet &EntityMappingProvider::getValuesOfType(
    EntityType entityType) const {
  if (entityType == EntityType::Variable) {
    return variableStorage->getAllValues();
  } else if (entityType == EntityType::Constant) {
    return constantStorage->getAllValues();
  } else if (entityType == EntityType::Procedure) {
    return procedureStorage->getProcedures();
  } else {
    // note: EntityType::None or Wildcard is invalid
    return HashKeySetTable<StmtValue, EntityValue>::getEmptyValue();
  }
}

EntityValue EntityMappingProvider::getVariableByIndex(EntityIdx index) const {
  return variableStorage->getValueByIdx(index);
}

EntityValue EntityMappingProvider::getConstantByIndex(EntityIdx index) const {
  return constantStorage->getValueByIdx(index);
}

EntityIdx EntityMappingProvider::getIndexOfVariable(EntityValue name) const {
  return variableStorage->getIdxOfValue(name);
}

EntityIdx EntityMappingProvider::getIndexOfConstant(EntityValue name) const {
  return constantStorage->getIdxOfValue(name);
}

bool EntityMappingProvider::isValueOfType(EntityType entityType,
                                          EntityValue name) const {
  if (entityType == EntityType::Variable) {
    return variableStorage->getIdxOfValue(name) != 0;
  } else if (entityType == EntityType::Constant) {
    return constantStorage->getIdxOfValue(name) != 0;
  } else if (entityType == EntityType::Procedure) {
    return procedureStorage->procedureExists(name);
  } else {
    return false;
  }
}
