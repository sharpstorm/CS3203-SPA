#include <memory>
#include <unordered_set>
#include <string>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/UsesWriter.h"

using std::make_shared, std::make_unique, std::unordered_set, std::string;

TEST_CASE("UsesWriter addUses") {
  auto table = make_shared<HashKeySetTable<int, string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  auto usesStorage = make_unique<UsesStorage>(table, reverseTable);
  auto pTable = make_shared<HashKeySetTable<string, string>>();
  auto reversePTable = make_shared<HashKeySetTable<string, string>>();
  auto usesPStorage = make_unique<UsesPStorage>(pTable, reversePTable);
  auto writer = UsesWriter(usesStorage.get(), usesPStorage.get());

  writer.addUses(1, "x", "main");
  writer.addUses(1, "y", "main");
  writer.addUses(3, "y", "main");
  writer.addUses(4, "y", "foo");

  REQUIRE(table->get(1) == unordered_set<string>({"x", "y"}));
  REQUIRE(table->get(3) == unordered_set<string>({"y"}));
  REQUIRE(reverseTable->get("y") == unordered_set<int>({1, 3, 4}));
  REQUIRE(reverseTable->get("x") == unordered_set<int>({1}));

  REQUIRE(pTable->get("main") == unordered_set<string>({"x", "y"}));
  REQUIRE(reversePTable->get("x") == unordered_set<string>({"main"}));
  REQUIRE(reversePTable->get("y") == unordered_set<string>({"main", "foo"}));
}
