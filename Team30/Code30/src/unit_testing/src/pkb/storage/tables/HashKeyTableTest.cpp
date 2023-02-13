#include <string>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeyTable.h"

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
