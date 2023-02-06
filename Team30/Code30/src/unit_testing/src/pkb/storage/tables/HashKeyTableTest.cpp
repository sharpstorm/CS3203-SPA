#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeyTable.h"

using std::string;
using std::unordered_set;

TEST_CASE("HashKeyTable with different key value types") {
  HashKeyTable<string, int> table;
  table.set("x", 1);
  table.set("y", 4);
  table.set("x", 3);

  REQUIRE(table.get("x") == unordered_set<int>{1, 3});
  REQUIRE(table.get("y") == unordered_set<int>{4});
}

TEST_CASE("HashKeyTable get unset key") {
  HashKeyTable<int, int> table;

  REQUIRE(table.get(1) == unordered_set<int>{});
}
