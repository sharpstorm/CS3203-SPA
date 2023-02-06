#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/writers/FollowsWriter.h"

using std::make_shared;
using std::unordered_set;

TEST_CASE("FollowsWriter addFollows") {
  auto table = make_shared<ContiguousTable<int>>();
  auto reverseTable = make_shared<ContiguousTable<int>>();
  auto store = new FollowsStorage(table, reverseTable);
  FollowsWriter writer = FollowsWriter(store);

  writer.addFollows(1, 3);
  writer.addFollows(3, 4);

  REQUIRE(table->get(1) == unordered_set<int>({3}));
  REQUIRE(reverseTable->get(3) == unordered_set<int>({1}));
  REQUIRE(table->get(3) == unordered_set<int>({4}));
}