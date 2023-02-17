#include <memory>
#include <unordered_set>
#include <string>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/UsesWriter.h"

using std::make_shared;
using std::make_unique;
using std::unordered_set;
using std::string;

TEST_CASE("UsesWriter addUses") {
  auto table = make_shared<HashKeySetTable<int, string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<UsesStorage>(table, reverseTable);
  auto writer = UsesWriter(store.get());

  writer.addUses(1, "x");
  writer.addUses(1, "y");
  writer.addUses(3, "y");

  REQUIRE(table->get(1) == unordered_set<string>({"x", "y"}));
  REQUIRE(table->get(3) == unordered_set<string>({"y"}));
  REQUIRE(reverseTable->get("y") == unordered_set<int>({1, 3}));
  REQUIRE(reverseTable->get("x") == unordered_set<int>({1}));
}
