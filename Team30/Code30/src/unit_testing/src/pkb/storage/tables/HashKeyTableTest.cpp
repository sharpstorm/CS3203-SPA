#include <string>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeyTable.h"
#include "common/Types.h"

using std::string;

TEST_CASE("HashKeyTable with different key value types") {
  HashKeyTable<string, int> table;
  table.set("x", 1);
  table.set("y", 4);

  REQUIRE(table.get("x") == 1);
  REQUIRE(table.get("y") == 4);
}

TEST_CASE("HashKeyTable get unset key") {
  HashKeyTable<int, int> table;

  REQUIRE(table.get(10) == 0);
}

TEST_CASE("HashKeyTable get unset StmtType") {
  HashKeyTable<int, StmtType> table;

  REQUIRE(table.get(10) == StmtType::None);
}

TEST_CASE("HashKeyTable get unset EntityType") {
  HashKeyTable<int, EntityType> table;

  REQUIRE(table.get(10) == EntityType::None);
}
