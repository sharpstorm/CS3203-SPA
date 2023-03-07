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
  auto table = make_shared<HashKeySetTable<string, string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, string>>();
  auto store = make_unique<CallsStorage>(table, reverseTable);

  auto stmtTable = make_shared<HashKeyTable<int, string>>();
  auto reverseStmtTable = make_shared<HashKeySetTable<string, int>>();
  auto stmtStore = make_unique<CallStmtStorage>(stmtTable, reverseStmtTable);
  auto writer = CallsWriter(store.get(), stmtStore.get());

  writer.addCalls(2, "main", "called");

  // call table
  REQUIRE(table->get("main") == unordered_set<string>({"called"}));
  REQUIRE(reverseTable->get("called") == unordered_set<string>({"main"}));

  // call stmt tables
  REQUIRE(stmtTable->get(2) == "called");
  REQUIRE(reverseStmtTable->get("called") == unordered_set<int>({2}));

}

