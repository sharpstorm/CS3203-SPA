#include <memory>

#include "catch.hpp"
#include "pkb/errors/PKBError.h"
#include "pkb/writers/ProcedureWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("ProcedureWriter addProcedure") {
  auto table = make_shared<ProcedureStmtTable>();
  auto procedureValues = make_shared<ProcedureValues>();
  auto callDeclarationTable = make_shared<CallDeclarationTable>();
  auto store = make_shared<ProcedureAndCallsStorage>(
      table.get(), callDeclarationTable.get(), procedureValues.get());
  auto writer = ProcedureWriter(store.get());

  writer.addProcedure("test1", 1, 3);
  writer.addProcedure("test2", 4, 6);

  REQUIRE(table->get(1) == "test1");
  REQUIRE(table->get(2) == "test1");
  REQUIRE(table->get(3) == "test1");
  REQUIRE(table->get(4) == "test2");
  REQUIRE(table->get(5) == "test2");
  REQUIRE(table->get(6) == "test2");
}

TEST_CASE("ProcedureWriter add duplicate procedure") {
  auto table = make_shared<ProcedureStmtTable>();
  auto procedureValues = make_shared<ProcedureValues>();
  auto callDeclarationTable = make_shared<CallDeclarationTable>();
  auto store = make_shared<ProcedureAndCallsStorage>(
      table.get(), callDeclarationTable.get(), procedureValues.get());
  auto writer = ProcedureWriter(store.get());

  writer.addProcedure("test1", 1, 3);

  REQUIRE_THROWS_AS(writer.addProcedure("test1", 4, 6), PKBError);
}
