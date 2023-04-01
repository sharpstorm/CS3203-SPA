#include "EntityMappingProviderStub.h"

#include "pkb/storage/tables/ContiguousSetTable.h"

EntityMappingProviderStub::EntityMappingProviderStub(){};

const EntitySet& EntityMappingProviderStub::getValuesOfType(
    EntityType type) const {
  if (type == EntityType::Variable) {
    return allVariables;
  } else if (type == EntityType::Constant) {
    return allConstants;
  } else if (type == EntityType::Procedure) {
    return allProcedures;
  } else {  // when type = Entity::None
    return ContiguousSetTable<EntityValue>::getEmptyValue();
  }
}

string EntityMappingProviderStub::getVariableByIndex(int index) const {
  return variableTable.get(index);
}

string EntityMappingProviderStub::getConstantByIndex(int index) const {
  return constantTable.get(index);
}

EntityIdx EntityMappingProviderStub::getIndexOfVariable(string name) const {
  return variableToIndexTable.get(name);
}

EntityIdx EntityMappingProviderStub::getIndexOfConstant(string name) const {
  return constantToIndexTable.get(name);
}

bool EntityMappingProviderStub::isValueOfType(EntityType type,
                                              string value) const {
  if (type == EntityType::Variable) {
    return variableToIndexTable.get(value) != 0;
  } else if (type == EntityType::Constant) {
    return variableToIndexTable.get(value) != 0;
  } else if (type == EntityType::Procedure) {
    return variableToIndexTable.get(value) != 0;
  } else {
    return false;
  }
}
