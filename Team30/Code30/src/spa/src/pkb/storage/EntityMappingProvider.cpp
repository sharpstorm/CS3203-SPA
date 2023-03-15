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

bool EntityMappingProvider::isValueOfType(string name,
                                          EntityType entityType) const {
  return true;
}

string EntityMappingProvider::getVariableByIndex(int index) const {
  return variableStorage->getByKey(index);
}

string EntityMappingProvider::getConstantByIndex(int index) const {
  return constantStorage->getByKey(index);
}

unordered_set<int> EntityMappingProvider::getIndexOfVariable(
    string name) const {
  return variableStorage->getByValue(name);
}

unordered_set<int> EntityMappingProvider::getIndexOfConstant(
    string name) const {
  return constantStorage->getByValue(name);
}
