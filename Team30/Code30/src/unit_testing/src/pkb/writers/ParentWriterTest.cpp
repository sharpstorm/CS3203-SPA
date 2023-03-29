#include <memory>
#include <set>

#include "catch.hpp"
#include "pkb/writers/ParentWriter.h"

using std::make_shared;
using std::make_unique;
using std::set;

TEST_CASE("ParentWriter addParent") {
  auto table = make_shared<ParentTable>();
  auto reverseTable = make_shared<ParentRevTable>();
  auto store = make_unique<ParentStorage>(table.get(), reverseTable.get());
  auto writer = ParentWriter(store.get());

  writer.addParent(1, 3);
  writer.addParent(3, 4);

  REQUIRE(table->get(1) == set<int>({3}));
  REQUIRE(reverseTable->get(3) == set<int>({1}));
  REQUIRE(table->get(3) == set<int>({4}));
}
