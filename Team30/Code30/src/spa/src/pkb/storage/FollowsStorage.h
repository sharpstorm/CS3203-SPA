#pragma once

#include "RelationTableManager.h"
#include "tables/ContiguousTable.h"

class FollowsStorage {
 public:
  FollowsStorage();
  ~FollowsStorage();
  void test() const;

 private:
  RelationTableManager<int, int> tableManager;
  ContiguousTable<int>* table;
  ContiguousTable<int>* reverseTable;
};
