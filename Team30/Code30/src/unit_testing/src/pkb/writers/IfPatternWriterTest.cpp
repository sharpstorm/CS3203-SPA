#include <memory>
#include <unordered_set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/IfPatternWriter.h"

using std::make_shared, std::make_unique, std::unordered_set, std::string;

TEST_CASE("IfPatternWriterTest addIfPattern") {
  auto table = make_shared<HashKeySetTable<int, string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<IfPatternStorage>(table, reverseTable);
  auto writer = IfPatternWriter(store.get());

  writer.addIfPattern(1, "x");
  writer.addIfPattern(1, "y");
  writer.addIfPattern(2, "x");
  writer.addIfPattern(3, "z");

  REQUIRE(table->get(1) == unordered_set<string>({"x", "y"}));
  REQUIRE(table->get(3) == unordered_set<string>({"z"}));
  REQUIRE(reverseTable->get("x") == unordered_set<int>({1, 2}));
  REQUIRE(reverseTable->get("z") == unordered_set<int>({3}));
}
