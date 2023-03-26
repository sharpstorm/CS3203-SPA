#include "VariableWriter.h"

VariableWriter::VariableWriter(VariableStorage *variableStorage)
    : variableStorage(variableStorage) {}

EntityIdx VariableWriter::addVariable(string variable) {
  return variableStorage->insert(variable);
}
