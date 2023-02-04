#pragma once

#include "../storage/StorageTypes.h"

class FollowsWriter {
 public:
  explicit FollowsWriter(FollowsStorage* followsStore);

  void addFollows(int arg1, int arg2);

 private:
  FollowsStorage* followsStore;
};
