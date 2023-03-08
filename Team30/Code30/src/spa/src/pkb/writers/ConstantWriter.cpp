#include "ConstantWriter.h"

ConstantWriter::ConstantWriter(ConstantStorage *constantStorage)
    : constantStorage(constantStorage) {}

void ConstantWriter::addConstant(string constant) {
  constantStorage->insert(constantStorage->getCurrIndex(), constant);
}
