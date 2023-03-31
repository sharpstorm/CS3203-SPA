#include <string>
#include <set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::string, std::set, std::make_pair;

TEST_CASE("HashKeySetTable with different key value types") {
  HashKeySetTable<string, int> table;
  table.insert("x", 1);
  table.insert("y", 4);
  table.insert("x", 3);

  REQUIRE(table.get("x") == set<int>{1, 3});
  REQUIRE(table.get("y") == set<int>{4});
}

TEST_CASE("HashKeySetTable get unset key") {
  HashKeySetTable<int, int> table;

  REQUIRE(table.get(10) == set<int>{});
}

