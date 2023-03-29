#pragma once

#include <string>

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IVariableWriter.h"

using std::string;

class VariableWriter : public IVariableWriter {
 public:
  explicit VariableWriter(VariableStorage *variableStorage);

  EntityIdx addVariable(string variable) override;

 private:
  VariableStorage *variableStorage;
};
