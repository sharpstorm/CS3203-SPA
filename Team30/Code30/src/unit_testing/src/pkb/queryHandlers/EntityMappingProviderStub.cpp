#include "EntityMappingProviderStub.h"

EntityMappingProviderStub::EntityMappingProviderStub() {};

unordered_set<string> EntityMappingProviderStub::getValuesOfType(
    EntityType type) const {
  if (type == EntityType::Variable) {
    return allVariables;
  } else if (type == EntityType::Constant) {
    return allConstants;
  } else if (type == EntityType::Procedure) {
    return allProcedures;
  } else {  // when type = Entity::None
    return {};
  }
}

string EntityMappingProviderStub::getVariableByIndex(int index) const {
  return variableTable.get(index);
}

string EntityMappingProviderStub::getConstantByIndex(int index) const {
  return constantTable.get(index);
}

unordered_set<int> EntityMappingProviderStub::getIndexOfVariable(
    string name) const {
  return variableToIndexTable.get(name);
}

unordered_set<int> EntityMappingProviderStub::getIndexOfConstant(
    string name) const {
  return constantToIndexTable.get(name);
}

bool EntityMappingProviderStub::isSymbolOfType(EntityType entityType,
                                               string name) const {
  if (entityType == EntityType::Variable) {
    return !variableToIndexTable.get(name).empty();
  } else if (entityType == EntityType::Constant) {
    return !constantToIndexTable.get(name).empty();
  } else if (entityType == EntityType::Procedure) {
    return !procedureToStmtNumTable.get(name).empty();
  } else {
    return false;
  }
}
