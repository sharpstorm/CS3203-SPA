#include <memory>

#include "catch.hpp"
#include "pkb/writers/PkbWriter.h"
#include "pkb/writers/postProcessors/CallsTPostProcessor.h"

using std::make_unique;

// One -> Two, Four
// Two -> Three, Four
// Three -> Four
// Five->Six
TEST_CASE("CallsTPostProcessor") {
  unique_ptr<PKB> pkb = make_unique<PKB>();
  PkbWriter writer(pkb.get());
  CallsTPostProcessor processor(pkb.get());

  // initialise ParentStorage and FollowsStorage
  // 1(->2(->3(->4,5|6)),7),8
  writer.addProcedure("One", 1, 2);
  writer.addProcedure("Two", 3, 4);
  writer.addProcedure("Three", 5, 5);
  writer.addProcedure("Four", 6, 6);
  writer.addProcedure("Five", 7, 7);
  writer.addProcedure("Six", 8, 8);
  writer.addCalls(1, "One", "Two");
  writer.addCalls(2, "One", "Four");
  writer.addCalls(3, "Two", "Three");
  writer.addCalls(4, "Two", "Four");
  writer.addCalls(5, "Three", "Four");
  writer.addCalls(7, "Five", "Six");

  REQUIRE(pkb->callsTTable->size() == 0);
  REQUIRE(pkb->callsTRevTable->size() == 0);

  processor.process();

  // check CallsTTable
  REQUIRE(pkb->callsTTable->size() == 4);
  auto t1 = pkb->callsTTable->get("One");
  REQUIRE(t1 == EntityValueSet({"Two", "Three", "Four"}));
  auto t2 = pkb->callsTTable->get("Two");
  REQUIRE(t2 == EntityValueSet({"Three", "Four"}));
  auto t3 = pkb->callsTTable->get("Three");
  REQUIRE(t3 == EntityValueSet({"Four"}));
  auto t4 = pkb->callsTTable->get("Four");
  REQUIRE(t4 == EntityValueSet({}));
  auto t5 = pkb->callsTTable->get("Five");
  REQUIRE(t5 == EntityValueSet({"Six"}));
  auto t6 = pkb->callsTTable->get("Six");
  REQUIRE(t6 == EntityValueSet({}));

  // check CallsTRevTable
  REQUIRE(pkb->callsTRevTable->size() == 4);
  auto r1 = pkb->callsTRevTable->get("One");
  REQUIRE(r1 == EntityValueSet({}));
  auto r2 = pkb->callsTRevTable->get("Two");
  REQUIRE(r2 == EntityValueSet({"One"}));
  auto r3 = pkb->callsTRevTable->get("Three");
  REQUIRE(r3 == EntityValueSet({"One", "Two"}));
  auto r4 = pkb->callsTRevTable->get("Four");
  REQUIRE(r4 == EntityValueSet({"One", "Two", "Three"}));
  auto r5 = pkb->callsTRevTable->get("Five");
  REQUIRE(r5 == EntityValueSet({}));
  auto r6 = pkb->callsTRevTable->get("Six");
  REQUIRE(r6 == EntityValueSet({"Five"}));
}
