#include "FollowsStorage.h"

#include <memory>

#include "tables/ContiguousTable.h"

FollowsStorage::FollowsStorage()
    : table(new ContiguousTable<int>()),
      reverseTable(new ContiguousTable<int>()),
      tableManager(RelationTableManager(table, reverseTable)) {}

FollowsStorage::~FollowsStorage() {
  delete table;
  delete reverseTable;
}

void FollowsStorage::test() const {}
