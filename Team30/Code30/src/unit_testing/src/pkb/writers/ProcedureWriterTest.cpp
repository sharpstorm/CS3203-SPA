#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/ProcedureWriter.h"
#include "pkb/errors/PKBError.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("ProcedureWriter addProcedure") {
  auto table = make_shared<EntityTable>();
  auto reverseTable = make_shared<EntityRevTable>();
  auto store = make_unique<ProcedureStorage>(table.get(), reverseTable.get());
  auto writer = ProcedureWriter(store.get());

  writer.addProcedure("test1", 1, 3);
  writer.addProcedure("test2", 4, 6);

  REQUIRE(table->get(1) == "test1");
  REQUIRE(table->get(2) == "test1");
  REQUIRE(table->get(3) == "test1");
  REQUIRE(table->get(4) == "test2");
  REQUIRE(table->get(5) == "test2");
  REQUIRE(table->get(6) == "test2");
  REQUIRE(reverseTable->get("test1") == unordered_set<int>({1, 2, 3}));
  REQUIRE(reverseTable->get("test2") == unordered_set<int>({4, 5, 6}));
}

TEST_CASE("ProcedureWriter add duplicate procedure") {
  auto table = make_shared<EntityTable>();
  auto reverseTable = make_shared<EntityRevTable>();
  auto store = make_unique<ProcedureStorage>(table.get(), reverseTable.get());
  auto writer = ProcedureWriter(store.get());

  writer.addProcedure("test1", 1, 3);

  REQUIRE_THROWS_AS(writer.addProcedure("test1", 4, 6), PKBError);
}
