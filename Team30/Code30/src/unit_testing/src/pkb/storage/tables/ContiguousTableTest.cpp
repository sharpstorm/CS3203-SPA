#include <string>
#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"

using std::string;
using std::unordered_set;

TEST_CASE("ContinuousTable set multiple values to key") {
  ContiguousTable<string> table;
  table.set(1, "a");
  table.set(1, "c");
  table.set(3, "b");

  REQUIRE(table.get(1) == unordered_set<string>{"a", "c"});
  REQUIRE(table.get(3) == unordered_set<string>{"b"});
}

TEST_CASE("ContinuousTable get unset key") {
  ContiguousTable<string> table;

  REQUIRE(table.get(1) == unordered_set<string>{});
}
