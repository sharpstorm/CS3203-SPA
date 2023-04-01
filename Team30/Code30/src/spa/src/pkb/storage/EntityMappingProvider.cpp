#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(
    VariableStorage *variableStorage, ConstantStorage *constantStorage,
    ProcedureAndCallsStorage *procedureStorage)
    : variableStorage(variableStorage),
      constantStorage(constantStorage),
      procedureStorage(procedureStorage) {}

const EntitySet &EntityMappingProvider::getValuesOfType(
    EntityType entityType) const {
  if (entityType == EntityType::Variable) {
    return variableStorage->getAllValues();
  } else if (entityType == EntityType::Constant) {
    return constantStorage->getAllValues();
  } else if (entityType == EntityType::Procedure) {
    return procedureStorage->getProcedures();
  } else {
    // note: EntityType::None is invalid
    return ContiguousSetTable<EntityValue>::getEmptyValue();
  }
}

EntityValue EntityMappingProvider::getVariableByIndex(int index) const {
  return variableStorage->getValueByIdx(index);
}

EntityValue EntityMappingProvider::getConstantByIndex(int index) const {
  return constantStorage->getValueByIdx(index);
}

EntityIdx EntityMappingProvider::getIndexOfVariable(string name) const {
  return variableStorage->getIdxOfValue(name);
}

EntityIdx EntityMappingProvider::getIndexOfConstant(string name) const {
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
