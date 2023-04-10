#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IParentWriter.h"

class ParentWriter : public IParentWriter {
 public:
  explicit ParentWriter(ParentStorage *parentStore);

  void addParent(StmtValue leftArg, StmtValue rightArg) override;

 private:
  ParentStorage *store;
};
