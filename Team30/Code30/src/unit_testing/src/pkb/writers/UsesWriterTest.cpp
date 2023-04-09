#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/writers/UsesWriter.h"

using std::make_shared, std::make_unique;

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

  REQUIRE(table->get(1) == EntityValueSet({"x", "y"}));
  REQUIRE(table->get(3) == EntityValueSet({"y"}));
  REQUIRE(reverseTable->get("y") == StmtValueSet({1, 3, 4}));
  REQUIRE(reverseTable->get("x") == StmtValueSet({1}));

  REQUIRE(pTable->get("main") == EntityValueSet({"x", "y"}));
  REQUIRE(reversePTable->get("x") == EntityValueSet({"main"}));
  REQUIRE(reversePTable->get("y") == EntityValueSet({"main", "foo"}));
}
