#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/writers/FollowsWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("FollowsWriter addFollows") {
  auto table = make_shared<FollowsTable>();
  auto reverseTable = make_shared<FollowsRevTable>();
  auto store = make_unique<FollowsStorage>(table.get(), reverseTable.get());
  auto writer = FollowsWriter(store.get());

  writer.addFollows(1, 3);
  writer.addFollows(3, 4);
  writer.addFollows(1, 4);

  REQUIRE(table->get(1) == StmtValueList({3, 4}));
  REQUIRE(table->get(3) == StmtValueList({4}));
  REQUIRE(reverseTable->get(3) == StmtValueList({1}));
  REQUIRE(reverseTable->get(4) == StmtValueList({3, 1}));
}
