#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/writers/IfPatternWriter.h"

using std::make_shared, std::make_unique, std::unordered_set;

TEST_CASE("IfPatternWriterTest addIfPattern") {
  auto table = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<IfPatternStorage>(table);
  auto writer = IfPatternWriter(store.get());

  writer.addIfPattern(1, "x");
  writer.addIfPattern(1, "y");
  writer.addIfPattern(2, "x");
  writer.addIfPattern(3, "z");

  REQUIRE(table->get("x") == unordered_set<int>({1, 2}));
  REQUIRE(table->get("z") == unordered_set<int>({3}));
}
