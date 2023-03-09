#include <memory>
#include <unordered_set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/WhilePatternWriter.h"

using std::make_shared, std::make_unique, std::unordered_set, std::string;

TEST_CASE("WhilePatternWriterTest addWhilePattern") {
  auto table = make_shared<HashKeySetTable<int, string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<WhilePatternStorage>(table, reverseTable);
  auto writer = WhilePatternWriter(store.get());

  writer.addWhilePattern(1, "x");
  writer.addWhilePattern(1, "y");
  writer.addWhilePattern(2, "x");
  writer.addWhilePattern(3, "z");

  REQUIRE(table->get(1) == unordered_set<string>({"x", "y"}));
  REQUIRE(table->get(3) == unordered_set<string>({"z"}));
  REQUIRE(reverseTable->get("x") == unordered_set<int>({1, 2}));
  REQUIRE(reverseTable->get("z") == unordered_set<int>({3}));
}
