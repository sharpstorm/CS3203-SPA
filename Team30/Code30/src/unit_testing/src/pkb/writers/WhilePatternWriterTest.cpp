#include <memory>
#include <set>
#include <string>

#include "catch.hpp"
#include "pkb/writers/WhilePatternWriter.h"

using std::make_shared, std::make_unique, std::set, std::string;

TEST_CASE("WhilePatternWriterTest addWhilePattern") {
  auto table = make_shared<WhilePatternTable>();
  auto reverseTable = make_shared<WhilePatternRevTable>();
  auto store = make_unique<WhilePatternStorage>(
      table.get(), reverseTable.get());
  auto writer = WhilePatternWriter(store.get());

  writer.addWhilePattern(1, "x");
  writer.addWhilePattern(1, "y");
  writer.addWhilePattern(2, "x");
  writer.addWhilePattern(3, "z");

  REQUIRE(table->get(1) == set<string>({"x", "y"}));
  REQUIRE(table->get(3) == set<string>({"z"}));
  REQUIRE(reverseTable->get("x") == set<int>({1, 2}));
  REQUIRE(reverseTable->get("z") == set<int>({3}));
}
