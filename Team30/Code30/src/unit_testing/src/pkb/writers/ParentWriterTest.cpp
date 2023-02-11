#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/writers/ParentWriter.h"

using std::make_shared;
using std::make_unique;
using std::unordered_set;

TEST_CASE("ParentWriter addParent") {
  auto table = make_shared<ContiguousTable<int>>();
  auto reverseTable = make_shared<ContiguousTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto writer = ParentWriter(store.get());

  writer.addParent(1, 3);
  writer.addParent(3, 4);

  REQUIRE(table->get(1) == unordered_set<int>({3}));
  REQUIRE(reverseTable->get(3) == unordered_set<int>({1}));
  REQUIRE(table->get(3) == unordered_set<int>({4}));
}
