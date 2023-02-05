#pragma once

#include "FollowsStorage.h"
#include "ParentStorage.h"

class PKB {
 public:
  PKB();
  FollowsStorage* followsStore;
  ParentStorage* parentStore;
};
