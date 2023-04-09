#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IConstantWriter.h"

class ConstantWriter : public IConstantWriter {
 public:
  explicit ConstantWriter(ConstantStorage *constantStorage);

  EntityIdx addConstant(EntityValue constant) override;

 private:
  ConstantStorage *constantStorage;
};
