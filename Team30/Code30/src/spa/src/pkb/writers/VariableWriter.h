#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IVariableWriter.h"

class VariableWriter : public IVariableWriter {
 public:
  explicit VariableWriter(VariableStorage *variableStorage);

  EntityIdx addVariable(EntityValue variable) override;

 private:
  VariableStorage *variableStorage;
};
