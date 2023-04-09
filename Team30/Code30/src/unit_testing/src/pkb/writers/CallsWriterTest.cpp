#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/writers/CallsWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("CallsWriter addCalls") {
  auto table = make_shared<CallsTable>();
  auto reverseTable = make_shared<CallsRevTable>();
  auto store = make_unique<CallsStorage>(table.get(), reverseTable.get());

  auto stmtTable = make_shared<CallDeclarationTable>();
  auto procedureValues = make_shared<ProcedureValues>();
  auto procAndCallsStorage = make_shared<ProcedureAndCallsStorage>(
      nullptr, stmtTable.get(), procedureValues.get());
  auto writer = CallsWriter(store.get(), procAndCallsStorage.get());

  writer.addCalls(2, "main", "called");

  // call table
  REQUIRE(table->get("main") == EntityValueSet({"called"}));
  REQUIRE(reverseTable->get("called") == EntityValueSet({"main"}));

  // call stmt tables
  REQUIRE(stmtTable->get(2) == "called");
}
