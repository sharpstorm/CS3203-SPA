#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_pair;

TEST_CASE("HashKeySetTable with different key value types") {
  HashKeySetTable<EntityValue, StmtValue> table;
  table.insert("x", 1);
  table.insert("y", 4);
  table.insert("x", 3);

  REQUIRE(table.get("x") == StmtValueSet{1, 3});
  REQUIRE(table.get("y") == StmtValueSet{4});
}

TEST_CASE("HashKeySetTable get unset key") {
  HashKeySetTable<int, int> table;

  REQUIRE(table.get(10) == StmtValueSet{});
}
