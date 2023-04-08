#include <memory>
#include <set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/ModifiesWriter.h"

using std::make_shared, std::make_unique, std::set, std::string;

TEST_CASE("ModifiesWriter addModifies") {
  auto table = make_shared<ModifiesTable>();
  auto reverseTable = make_shared<ModifiesRevTable>();
  auto modifiesStorage =
      make_unique<ModifiesStorage>(table.get(), reverseTable.get());
  auto pTable = make_shared<ModifiesPTable>();
  auto reversePTable = make_shared<ModifiesPRevTable>();
  auto modifiesPStorage =
      make_unique<ModifiesPStorage>(pTable.get(), reversePTable.get());
  auto writer = ModifiesWriter(modifiesStorage.get(), modifiesPStorage.get());

  writer.addModifies(1, "x", "main");
  writer.addModifies(1, "y", "main");
  writer.addModifies(3, "y", "main");
  writer.addModifies(4, "y", "foo");

  REQUIRE(table->get(1) == unordered_set<string>({"x", "y"}));
  REQUIRE(table->get(3) == unordered_set<string>({"y"}));
  REQUIRE(reverseTable->get("y") == unordered_set<int>({1, 3, 4}));
  REQUIRE(reverseTable->get("x") == unordered_set<int>({1}));

  REQUIRE(pTable->get("main") == unordered_set<string>({"x", "y"}));
  REQUIRE(reversePTable->get("x") == unordered_set<string>({"main"}));
  REQUIRE(reversePTable->get("y") == unordered_set<string>({"main", "foo"}));
}
