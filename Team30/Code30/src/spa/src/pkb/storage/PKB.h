#pragma once

#include "FollowsStorage.h"
#include "ParentStorage.h"
#include "SymbolStorage.h"

class PKB {
 public:
  PKB();
  static FollowsStorage* followsStore;
  static ParentStorage* parentStore;
  static SymbolStorage* SymbolStorage;
};
