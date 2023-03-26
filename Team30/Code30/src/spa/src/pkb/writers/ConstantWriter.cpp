#include "ConstantWriter.h"

ConstantWriter::ConstantWriter(ConstantStorage *constantStorage)
    : constantStorage(constantStorage) {}

EntityIdx ConstantWriter::addConstant(string constant) {
  return constantStorage->insert(constant);
}
