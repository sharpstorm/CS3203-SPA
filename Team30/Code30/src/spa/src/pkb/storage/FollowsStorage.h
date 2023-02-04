#pragma once

#include <memory>

#include "RelationTableManager.h"
#include "tables/ContiguousTable.h"
class FollowsStorageX {
 private:
  RelationTableManager<int, int> tableManager;

 public:
  FollowsStorageX();

  void test() const;
};
