#include <string>
#include <utility>

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

TEST_CASE("HashKeyTable begin end iterator") {
  HashKeyTable<int, string> table;

  table.set(1, "x");
  table.set(3, "y");
  table.set(10, "z");

  pair_set<int, string> allPairs;
  for (auto itr = table.begin(); itr != table.end(); ++itr) {
    allPairs.insert({itr->first, itr->second});
  };
  REQUIRE(allPairs == pair_set<int, string>({{1, "x"}, {3, "y"}, {10, "z"}}));
}
