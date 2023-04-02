#include <memory>
#include <vector>

#include "catch.hpp"
#include "pkb/writers/FollowsWriter.h"

using std::make_shared;
using std::make_unique;
using std::vector;

TEST_CASE("FollowsWriter addFollows") {
  auto table = make_shared<FollowsTable>();
  auto reverseTable = make_shared<FollowsRevTable>();
  auto store = make_unique<FollowsStorage>(table.get(), reverseTable.get());
  auto writer = FollowsWriter(store.get());

  writer.addFollows(1, 3);
  writer.addFollows(3, 4);
  writer.addFollows(1, 4);

  REQUIRE(table->get(1) == vector<int>({3, 4}));
  REQUIRE(table->get(3) == vector<int>({4}));
  REQUIRE(reverseTable->get(3) == vector<int>({1}));
  REQUIRE(reverseTable->get(4) == vector<int>({3, 1}));
}
