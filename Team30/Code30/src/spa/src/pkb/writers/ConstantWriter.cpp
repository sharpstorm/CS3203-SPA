#include "ConstantWriter.h"

ConstantWriter::ConstantWriter(ConstantStorage *constantStorage)
    : constantStorage(constantStorage) {}

EntityIdx ConstantWriter::addConstant(EntityValue constant) {
  return constantStorage->insert(constant);
}
