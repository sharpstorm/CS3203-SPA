#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/writers/ParentWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("ParentWriter addParent") {
  auto table = make_shared<ParentTable>();
  auto reverseTable = make_shared<ParentRevTable>();
  auto store = make_unique<ParentStorage>(table.get(), reverseTable.get());
  auto writer = ParentWriter(store.get());

  writer.addParent(1, 3);
  writer.addParent(3, 4);

  REQUIRE(table->get(1) == StmtValueSet({3}));
  REQUIRE(reverseTable->get(3) == StmtValueSet({1}));
  REQUIRE(table->get(3) == StmtValueSet({4}));
}
