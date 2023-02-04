#include "FollowsStorage.h"

#include <memory>

#include "tables/ContiguousTable.h"

using std::make_shared;

FollowsStorageX::FollowsStorageX()
    : tableManager(RelationTableManager<int, int>(
          make_shared<ContiguousTable<int>>(),
          make_shared<ContiguousTable<int>>())) {}

void FollowsStorageX::test() const {}
