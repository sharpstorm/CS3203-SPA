#include "VariableWriter.h"

VariableWriter::VariableWriter(VariableStorage *variableStorage)
    : variableStorage(variableStorage) {}

EntityIdx VariableWriter::addVariable(EntityValue variable) {
  return variableStorage->insert(variable);
}
