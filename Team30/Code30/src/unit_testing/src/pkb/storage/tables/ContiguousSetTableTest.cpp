#include <string>
#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousSetTable.h"

using std::string;
using std::unordered_set;

TEST_CASE("ContinuousSetTable set multiple values to key") {
  ContiguousSetTable<string> table(5);
  table.set(1, "a");
  table.set(1, "b");
  table.set(1, "c");
  table.set(3, "d");

  REQUIRE(table.get(1) == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(table.get(3) == unordered_set<string>({"d"}));
}

TEST_CASE("ContinuousSetTable get unset key") {
  ContiguousSetTable<string> table(2);

  REQUIRE(table.get(1) == unordered_set<string>({}));
  REQUIRE(table.get(10) == unordered_set<string>({}));
}
