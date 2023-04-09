#include <memory>

#include "catch.hpp"
#include "pkb/writers/WhilePatternWriter.h"

using std::make_shared, std::make_unique;

TEST_CASE("WhilePatternWriterTest addWhilePattern") {
  auto table = make_shared<WhilePatternTable>();
  auto reverseTable = make_shared<WhilePatternRevTable>();
  auto store =
      make_unique<WhilePatternStorage>(table.get(), reverseTable.get());
  auto writer = WhilePatternWriter(store.get());

  writer.addWhilePattern(1, "x");
  writer.addWhilePattern(1, "y");
  writer.addWhilePattern(2, "x");
  writer.addWhilePattern(3, "z");

  REQUIRE(table->get(1) == EntityValueSet({"x", "y"}));
  REQUIRE(table->get(3) == EntityValueSet({"z"}));
  REQUIRE(reverseTable->get("x") == StmtValueSet({1, 2}));
  REQUIRE(reverseTable->get("z") == StmtValueSet({3}));
}
