#include <memory>
#include <set>

#include "catch.hpp"
#include "pkb/writers/FollowsWriter.h"

using std::make_shared;
using std::make_unique;
using std::set;

TEST_CASE("FollowsWriter addFollows") {
  auto table = make_shared<FollowsTable>();
  auto reverseTable = make_shared<FollowsRevTable>();
  auto store = make_unique<FollowsStorage>(table.get(), reverseTable.get());
  auto writer = FollowsWriter(store.get());

  writer.addFollows(1, 3);
  writer.addFollows(3, 4);

  REQUIRE(table->get(1) == set<int>({3}));
  REQUIRE(reverseTable->get(3) == set<int>({1}));
  REQUIRE(table->get(3) == set<int>({4}));
}
