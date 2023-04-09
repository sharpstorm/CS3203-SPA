#include "EntityMappingProviderStub.h"

#include "pkb/storage/tables/HashKeySetTable.h"

EntityMappingProviderStub::EntityMappingProviderStub(){};

const EntityValueSet& EntityMappingProviderStub::getValuesOfType(
    EntityType type) const {
  if (type == EntityType::Variable) {
    return allVariables;
  } else if (type == EntityType::Constant) {
    return allConstants;
  } else if (type == EntityType::Procedure) {
    return allProcedures;
  } else {  // when type = Entity::None
    return HashKeySetTable<StmtValue, EntityValue>::getEmptyValue();
  }
}

EntityValue EntityMappingProviderStub::getVariableByIndex(
    EntityIdx index) const {
  return variableTable.get(index);
}

EntityValue EntityMappingProviderStub::getConstantByIndex(
    EntityIdx index) const {
  return constantTable.get(index);
}

EntityIdx EntityMappingProviderStub::getIndexOfVariable(
    EntityValue name) const {
  return variableToIndexTable.get(name);
}

EntityIdx EntityMappingProviderStub::getIndexOfConstant(
    EntityValue name) const {
  return constantToIndexTable.get(name);
}

bool EntityMappingProviderStub::isValueOfType(EntityType type,
                                              EntityValue value) const {
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
