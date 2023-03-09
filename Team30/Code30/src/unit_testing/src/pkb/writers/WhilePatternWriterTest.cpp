#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/writers/WhilePatternWriter.h"

using std::make_shared, std::make_unique, std::unordered_set;

TEST_CASE("WhilePatternWriterTest addWhilePattern") {
  auto table = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<WhilePatternStorage>(table);
  auto writer = WhilePatternWriter(store.get());

  writer.addWhilePattern(1, "x");
  writer.addWhilePattern(1, "y");
  writer.addWhilePattern(2, "x");
  writer.addWhilePattern(3, "z");

  REQUIRE(table->get("x") == unordered_set<int>({1, 2}));
  REQUIRE(table->get("z") == unordered_set<int>({3}));
}
