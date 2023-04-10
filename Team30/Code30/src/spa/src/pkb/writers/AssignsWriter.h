#pragma once

#include <memory>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IAssignsWriter.h"

class AssignsWriter : public IAssignsWriter {
 public:
  explicit AssignsWriter(AssignStorage *assignStorage);
  void addAssigns(StmtValue leftArg, PatternTrieSPtr rightArg) override;

 private:
  AssignStorage *assignStorage;
};
