#pragma once

#include "../storage/StorageTypes.h"

class ParentWriter {
 public:
  explicit ParentWriter(ParentStorage* parentStore);

  void addParent(int arg1, int arg2);

 private:
  ParentStorage* store;
};
