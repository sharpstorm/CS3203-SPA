#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::string, std::unordered_set, std::make_pair;

TEST_CASE("HashKeySetTable with different key value types") {
  HashKeySetTable<string, int> table;
  table.set("x", 1);
  table.set("y", 4);
  table.set("x", 3);

  REQUIRE(table.get("x") == unordered_set<int>{1, 3});
  REQUIRE(table.get("y") == unordered_set<int>{4});
}

TEST_CASE("HashKeySetTable get unset key") {
  HashKeySetTable<int, int> table;

  REQUIRE(table.get(10) == unordered_set<int>{});
}

