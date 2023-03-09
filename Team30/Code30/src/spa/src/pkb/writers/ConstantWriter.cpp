#include "ConstantWriter.h"

ConstantWriter::ConstantWriter(ConstantStorage *constantStorage)
    : constantStorage(constantStorage), index(1) {}

void ConstantWriter::addConstant(string constant) {
  constantStorage->insert(index++, constant);
}
