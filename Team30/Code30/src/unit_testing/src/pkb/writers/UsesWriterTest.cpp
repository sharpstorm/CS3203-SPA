#include <memory>
#include <unordered_set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/UsesWriter.h"

using std::make_shared, std::make_unique, std::unordered_set, std::string;

TEST_CASE("UsesWriter addUses") {
  auto table = make_shared<UsesTable>();
  auto reverseTable = make_shared<UsesRevTable>();
  auto usesStorage = make_unique<UsesStorage>(table.get(), reverseTable.get());
  auto pTable = make_shared<UsesPTable>();
  auto reversePTable = make_shared<UsesPRevTable>();
  auto usesPStorage =
      make_unique<UsesPStorage>(pTable.get(), reversePTable.get());
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
