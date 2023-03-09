#include "VariableWriter.h"

VariableWriter::VariableWriter(VariableStorage *variableStorage)
    : variableStorage(variableStorage), index(1) {}

void VariableWriter::addVariable(string variable) {
  variableStorage->insert(index++, variable);
}
