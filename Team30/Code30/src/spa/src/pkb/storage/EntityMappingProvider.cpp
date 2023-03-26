#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(VariableStorage *variableStorage,
                                             ConstantStorage *constantStorage,
                                             ProcedureStorage *procedureStorage)
    : variableStorage(variableStorage),
      constantStorage(constantStorage),
      procedureStorage(procedureStorage) {}

unordered_set<string> EntityMappingProvider::getValuesOfType(
    EntityType entityType) const {
  if (entityType == EntityType::Variable) {
    return variableStorage->getAllValues();
  } else if (entityType == EntityType::Constant) {
    return constantStorage->getAllValues();
  } else if (entityType == EntityType::Procedure) {
    return procedureStorage->getAllValues();
  } else {
    // note: EntityType::None is invalid
    return {};
  }
}

string EntityMappingProvider::getVariableByIndex(int index) const {
  return variableStorage->getValueByIdx(index);
}

string EntityMappingProvider::getConstantByIndex(int index) const {
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
    return !procedureStorage->getByValue(name).empty();
  } else {
    return false;
  }
}
