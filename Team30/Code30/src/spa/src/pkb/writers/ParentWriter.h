#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IParentWriter.h"

class ParentWriter : public IParentWriter {
 public:
  explicit ParentWriter(ParentStorage *parentStore);

  void addParent(int arg1, int arg2) override;

 private:
  ParentStorage *store;
};
