#include <memory>
#include <set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/CallsWriter.h"

using std::make_shared;
using std::make_unique;
using std::set;
using std::string;

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
  REQUIRE(table->get("main") == unordered_set<string>({"called"}));
  REQUIRE(reverseTable->get("called") == unordered_set<string>({"main"}));

  // call stmt tables
  REQUIRE(stmtTable->get(2) == "called");
}
