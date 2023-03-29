#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"

using std::string;

TEST_CASE("ContinuousTable set to key beyond table initial size") {
  ContiguousTable<string> table(1);
  table.insert(1, "a");
  table.insert(2, "b");
  table.insert(3, "c");

  REQUIRE(table.get(1) == "a");
  REQUIRE(table.get(2) == "b");
  REQUIRE(table.get(3) == "c");
}

TEST_CASE("ContinuousTable get unset key") {
  ContiguousTable<string> table(2);

  REQUIRE(table.get(1) == "");
  REQUIRE(table.get(10) == "");
}

TEST_CASE("ContinuousTable store set of values") {
  ContiguousTable<std::unordered_set<string>> table(5);
  table.insert(1, {"a"});

  REQUIRE(table.get(1) == std::unordered_set<string>({"a"}));
  REQUIRE(table.get(10) == std::unordered_set<string>({}));
}

