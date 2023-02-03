#pragma once

#include "FollowsStorage.h"
#include "ParentStorage.h"

class PKB {
 public:
  PKB();
  static FollowsStorage* followsStore;
  static ParentStorage* parentStore;
};
