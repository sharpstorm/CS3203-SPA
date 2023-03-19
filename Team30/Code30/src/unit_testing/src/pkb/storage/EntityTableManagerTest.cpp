
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/EntityTableManager.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_shared;
using std::pair;
using std::string;
using std::unordered_set;

TEST_CASE("EntityTableManager<string, EntityType>") {
  auto table = make_shared<HashKeyTable<string, EntityType>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityType, string>>();
  EntityTableManager<string, EntityType>
      tableManager(table.get(), reverseTable.get());

  tableManager.insert("test1", EntityType::Variable);
  tableManager.insert("test2", EntityType::Variable);

  REQUIRE(tableManager.getByKey("test1") == EntityType::Variable);
  REQUIRE(tableManager.getByValue(EntityType::Variable) ==
      unordered_set<string>({"test1", "test2"}));

}

TEST_CASE("EntityTableManager<int, StmtType>") {
  auto table = make_shared<ContiguousTable<StmtType>>();
  auto reverseTable = make_shared<HashKeySetTable<StmtType, int>>();
  EntityTableManager<int, StmtType>
      tableManager(table.get(), reverseTable.get());
  tableManager.insert(1, StmtType::Assign);
  tableManager.insert(2, StmtType::Assign);

  REQUIRE(tableManager.getByKey(1) == StmtType::Assign);
  REQUIRE(tableManager.getByValue(StmtType::Assign) ==
      unordered_set<int>({1, 2}));

}

TEST_CASE("EntityTableManager insert and getAllKeys, getAllValues") {
  auto table = make_shared<ContiguousTable<StmtType>>();
  auto reverseTable = make_shared<HashKeySetTable<StmtType, int>>();
  EntityTableManager<int, StmtType>
      tableManager(table.get(), reverseTable.get());
  tableManager.insert(1, StmtType::Assign);
  tableManager.insert(2, StmtType::Print);
  tableManager.insert(3, StmtType::Assign);

  REQUIRE(tableManager.getAllKeys() == unordered_set<int>({1, 2, 3}));
  REQUIRE(tableManager.getAllValues() ==
      unordered_set<StmtType>({StmtType::Assign, StmtType::Print}));
}

TEST_CASE("EntityTableManager insertFromTo and getByKey, getByValue") {
  auto table = make_shared<ContiguousTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  EntityTableManager<int, string> tableManager(table.get(), reverseTable.get());
  tableManager.insertFromTo(1, 4, "procedureA");
  tableManager.insertFromTo(5, 8, "procedureB");

  REQUIRE(tableManager.getByKey(1) == "procedureA");
  REQUIRE(tableManager.getByKey(2) == "procedureA");
  REQUIRE(tableManager.getByKey(3) == "procedureA");
  REQUIRE(tableManager.getByKey(4) == "procedureA");
  REQUIRE(tableManager.getByKey(5) == "procedureB");
  REQUIRE(tableManager.getByKey(6) == "procedureB");
  REQUIRE(tableManager.getByKey(7) == "procedureB");
  REQUIRE(tableManager.getByKey(8) == "procedureB");
  REQUIRE(tableManager.getByValue("procedureA") ==
      unordered_set<int>({1, 2, 3, 4}));
  REQUIRE(tableManager.getByValue("procedureB") ==
      unordered_set<int>({5, 6, 7, 8}));
}

TEST_CASE("EntityTableManager insertFromTo and getAllKeys, getAllValues") {
  auto table = make_shared<ContiguousTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  EntityTableManager<int, string> tableManager(table.get(), reverseTable.get());
  tableManager.insertFromTo(1, 4, "procedureA");

  REQUIRE(tableManager.getAllKeys() == unordered_set<int>({1, 2, 3, 4}));
  REQUIRE(tableManager.getAllValues() == unordered_set<string>({"procedureA"}));
}

