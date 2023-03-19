#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/CallsWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("CallsWriter addCalls") {
  auto table = make_shared<CallsTable>();
  auto reverseTable = make_shared<CallsRevTable>();
  auto store = make_unique<CallsStorage>(table.get(), reverseTable.get());

  auto stmtTable = make_shared<CallStmtTable>();
  auto reverseStmtTable = make_shared<CallStmtRevTable>();
  auto stmtStore = make_unique<CallStmtStorage>(stmtTable.get(),
                                                reverseStmtTable.get());
  auto writer = CallsWriter(store.get(), stmtStore.get());

  writer.addCalls(2, "main", "called");

  // call table
  REQUIRE(table->get("main") == unordered_set<string>({"called"}));
  REQUIRE(reverseTable->get("called") == unordered_set<string>({"main"}));

  // call stmt tables
  REQUIRE(stmtTable->get(2) == "called");
  REQUIRE(reverseStmtTable->get("called") == unordered_set<int>({2}));

}

