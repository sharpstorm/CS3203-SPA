#pragma once

#include <string>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IConstantWriter.h"

using std::string;

class ConstantWriter : public IConstantWriter {
 public:
  explicit ConstantWriter(ConstantStorage *constantStorage);

  void addConstant(string constant) override;

 private:
  ConstantStorage *constantStorage;
  int index;
};
