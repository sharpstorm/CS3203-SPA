#include <memory>
#include <set>

#include "catch.hpp"
#include "pkb/writers/PkbWriter.h"
#include "pkb/writers/postProcessors/ParentTPostProcessor.h"

using std::make_unique;

TEST_CASE("ParentTPostProcessor") {
  unique_ptr<PKB> pkb = make_unique<PKB>();
  PkbWriter writer(pkb.get());
  ParentTPostProcessor processor(pkb.get());

  // initialise ParentStorage and FollowsStorage
  // 1(->2(->3(->4,5|6)),7),8
  writer.addParent(1, 2);
  writer.addParent(1, 7);
  writer.addParent(2, 3);
  writer.addParent(3, 4);
  writer.addParent(3, 5);
  writer.addParent(3, 6);
  writer.addFollows(1, 8);
  writer.addFollows(2, 7);
  writer.addFollows(4, 5);

  REQUIRE(pkb->parentTTable->size() == 0);
  REQUIRE(pkb->parentTRevTable->size() == 0);

  processor.process();

  // check ParentTTable
  REQUIRE(pkb->parentTTable->size() == 3);
  auto t1 = pkb->parentTTable->get(1);
  REQUIRE(t1 == 7);
  auto t2 = pkb->parentTTable->get(2);
  REQUIRE(t2 == 6);
  auto t3 = pkb->parentTTable->get(3);
  REQUIRE(t3 == 6);

  // check ParentTRevTable
  REQUIRE(pkb->parentTRevTable->size() == 4);
  auto r1 = pkb->parentTRevTable->get(7);
  REQUIRE(r1 == StmtValueSet({1}));
  auto r2 = pkb->parentTRevTable->get(3);
  REQUIRE(r2 == StmtValueSet({1, 2}));
  auto r3 = pkb->parentTRevTable->get(5);
  REQUIRE(r3 == StmtValueSet({1, 2, 3}));
  auto r4 = pkb->parentTRevTable->get(6);
  REQUIRE(r4 == StmtValueSet({1, 2, 3}));
}
