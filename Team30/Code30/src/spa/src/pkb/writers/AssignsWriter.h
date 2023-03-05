#pragma once

#include <memory>
#include <string>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IAssignsWriter.h"

using std::string;

class AssignsWriter : public IAssignsWriter {
 public:
  explicit AssignsWriter(AssignStorage *assignStorage);
  void addAssigns(int arg1, PatternTrieSPtr arg2) override;

 private:
  AssignStorage *assignStorage;
};
