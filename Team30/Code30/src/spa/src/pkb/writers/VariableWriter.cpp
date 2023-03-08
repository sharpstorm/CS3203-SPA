#include "VariableWriter.h"

VariableWriter::VariableWriter(VariableStorage *variableStorage)
    : variableStorage(variableStorage) {}

void VariableWriter::addVariable(string variable) {
  variableStorage->insert(variableStorage->getCurrIndex(), variable);
}
