#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/tables/ContiguousSetTable.h"

TEST_CASE("ContinuousSetTable set multiple values to key") {
  ContiguousSetTable<EntityValue> table(5);
  table.insert(1, "a");
  table.insert(1, "b");
  table.insert(1, "c");
  table.insert(3, "d");

  REQUIRE(table.get(1) == EntityValueSet({"a", "b", "c"}));
  REQUIRE(table.get(3) == EntityValueSet({"d"}));
}

TEST_CASE("ContinuousSetTable get unset key") {
  ContiguousSetTable<string> table(2);

  REQUIRE(table.get(1) == EntityValueSet({}));
  REQUIRE(table.get(10) == EntityValueSet({}));
}
