#include <memory>

#include "catch.hpp"
#include "pkb/writers/ModifiesWriter.h"

using std::make_shared, std::make_unique;

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

  REQUIRE(table->get(1) == EntityValueSet({"x", "y"}));
  REQUIRE(table->get(3) == EntityValueSet({"y"}));
  REQUIRE(reverseTable->get("y") == StmtValueSet({1, 3, 4}));
  REQUIRE(reverseTable->get("x") == StmtValueSet({1}));

  REQUIRE(pTable->get("main") == EntityValueSet({"x", "y"}));
  REQUIRE(reversePTable->get("x") == EntityValueSet({"main"}));
  REQUIRE(reversePTable->get("y") == EntityValueSet({"main", "foo"}));
}
