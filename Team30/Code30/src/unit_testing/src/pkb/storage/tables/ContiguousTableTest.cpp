#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/tables/ContiguousTable.h"

TEST_CASE("ContinuousTable set to key beyond table initial size") {
  ContiguousTable<EntityValue> table(1);
  table.insert(1, "a");
  table.insert(2, "b");
  table.insert(3, "c");

  REQUIRE(table.get(1) == "a");
  REQUIRE(table.get(2) == "b");
  REQUIRE(table.get(3) == "c");
}

TEST_CASE("ContinuousTable get unset key") {
  ContiguousTable<EntityValue> table(2);

  REQUIRE(table.get(1) == "");
  REQUIRE(table.get(10) == "");
}

TEST_CASE("ContinuousTable store set of values") {
  ContiguousTable<EntityValueSet> table(5);
  table.insert(1, {"a"});

  REQUIRE(table.get(1) == EntityValueSet({"a"}));
  REQUIRE(table.get(10) == EntityValueSet({}));
}
