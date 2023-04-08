
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/tables/HashKeyTable.h"

TEST_CASE("HashKeyTable with different key value types") {
  HashKeyTable<EntityValue, StmtValue> table;
  table.insert("x", 1);
  table.insert("y", 4);

  REQUIRE(table.get("x") == 1);
  REQUIRE(table.get("y") == 4);
}

TEST_CASE("HashKeyTable get unset key") {
  HashKeyTable<StmtValue, StmtValue> table;

  REQUIRE(table.get(10) == 0);
}

TEST_CASE("HashKeyTable get unset StmtType") {
  HashKeyTable<StmtValue, StmtType> table;

  REQUIRE(table.get(10) == StmtType::None);
}

TEST_CASE("HashKeyTable get unset EntityType") {
  HashKeyTable<StmtValue, EntityType> table;

  REQUIRE(table.get(10) == EntityType::None);
}
