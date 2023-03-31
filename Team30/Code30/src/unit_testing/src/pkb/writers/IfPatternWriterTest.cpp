#include <memory>
#include <set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/IfPatternWriter.h"

using std::make_shared, std::make_unique, std::set, std::string;

TEST_CASE("IfPatternWriterTest addIfPattern") {
  auto table = make_shared<IfPatternTable>();
  auto reverseTable = make_shared<IfPatternRevTable>();
  auto store = make_unique<IfPatternStorage>(table.get(), reverseTable.get());
  auto writer = IfPatternWriter(store.get());

  writer.addIfPattern(1, "x");
  writer.addIfPattern(1, "y");
  writer.addIfPattern(2, "x");
  writer.addIfPattern(3, "z");

  REQUIRE(table->get(1) == set<string>({"x", "y"}));
  REQUIRE(table->get(3) == set<string>({"z"}));
  REQUIRE(reverseTable->get("x") == set<int>({1, 2}));
  REQUIRE(reverseTable->get("z") == set<int>({3}));
}
