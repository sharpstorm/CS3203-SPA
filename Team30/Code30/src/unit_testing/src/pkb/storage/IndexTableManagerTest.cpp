#include <memory>

#include "catch.hpp"
#include "pkb/storage/IndexTableManager.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeyTable.h"

using std::make_unique;

TEST_CASE("IndexTableManager empty") {
  auto table = make_unique<ContiguousTable<EntityValue>>();
  auto reverseTable = make_unique<HashKeyTable<EntityValue, EntityIdx>>();
  auto values = make_unique<EntityValueSet>();
  IndexTableManager<EntityValue> tableManager(table.get(), reverseTable.get(),
                                              values.get());

  REQUIRE(tableManager.getValueByIdx(1) == "");
  REQUIRE(tableManager.getIdxOfValue("x") == 0);
}

TEST_CASE("IndexTableManager <EntityValue>") {
  auto table = make_unique<ContiguousTable<EntityValue>>();
  auto reverseTable = make_unique<HashKeyTable<EntityValue, EntityIdx>>();
  auto values = make_unique<EntityValueSet>();
  IndexTableManager<EntityValue> tableManager(table.get(), reverseTable.get(),
                                              values.get());

  auto idx1 = tableManager.insert("abc");
  auto idx2 = tableManager.insert("def");
  auto idx3 = tableManager.insert("abc");

  // write test
  REQUIRE(idx1 == 1);
  REQUIRE(idx2 == 2);
  REQUIRE(idx3 == 1);
  // storage test
  REQUIRE(table->get(1) == "abc");
  REQUIRE(table->get(2) == "def");
  REQUIRE(reverseTable->get("abc") == 1);
  REQUIRE(reverseTable->get("def") == 2);
  // query test
  REQUIRE(tableManager.getValueByIdx(1) == "abc");
  REQUIRE(tableManager.getValueByIdx(2) == "def");
  REQUIRE(tableManager.getIdxOfValue("abc") == 1);
  REQUIRE(tableManager.getIdxOfValue("def") == 2);
  // not found
  REQUIRE(tableManager.getValueByIdx(10) == "");
  REQUIRE(tableManager.getIdxOfValue("x") == 0);
}
