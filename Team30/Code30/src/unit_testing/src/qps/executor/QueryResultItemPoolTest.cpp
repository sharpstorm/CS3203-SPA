#include "catch.hpp"
#include "qps/common/intermediate_result/QueryResultItemPool.h"

TEST_CASE("Test QueryResultItem Pool Adoption") {
  QueryResultItemPool poolA;
  QueryResultItemPool poolB;

  // Build Pool A
  auto ptr1 = poolA.getItem(1);
  auto ptrA = poolA.getItem("a");

  // Build Pool B
  auto ptrB1 = poolB.getItem(1);
  auto ptr4 = poolB.getItem(4);

  auto ptrBa = poolB.getItem("a");
  auto ptrD = poolB.getItem("d");

  REQUIRE(ptr1 == poolA.getItem(1));
  REQUIRE(ptrA == poolA.getItem("a"));
  REQUIRE(ptrB1 == poolB.getItem(1));
  REQUIRE(ptrBa == poolB.getItem("a"));

  REQUIRE(ptr4 == poolB.getItem(4));
  REQUIRE(ptrD == poolB.getItem("d"));

  auto orphans = poolA.adoptPool(&poolB);
  REQUIRE(ptr4 == poolA.getItem(4));
  REQUIRE(ptrD == poolA.getItem("d"));

  REQUIRE(orphans->getMappingFor(ptrB1) == ptr1);
  REQUIRE(orphans->getMappingFor(ptrBa) == ptrA);
}
