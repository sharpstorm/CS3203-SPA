
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "pkb/predicates/Predicate.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/TransitiveRelationTableManager.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_pair;
using std::make_shared;
using std::string;
using std::unordered_set;

TEST_CASE("TransitiveRelationTableManager getByFirstArgT") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(1, 2);
  tableManager.insert(1, 3);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 6);

  REQUIRE(tableManager.getByFirstArgT(6) == unordered_set<int>({}));
  REQUIRE(tableManager.getByFirstArgT(5) == unordered_set<int>({6}));
  REQUIRE(tableManager.getByFirstArgT(1) ==
      unordered_set<int>({2, 3, 4, 5, 6}));
}

TEST_CASE("TransitiveRelationTableManager getBySecondArgT") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(2, 1);
  tableManager.insert(3, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(6, 5);

  REQUIRE(tableManager.getBySecondArgT(6) == unordered_set<int>({}));
  REQUIRE(tableManager.getBySecondArgT(5) == unordered_set<int>({6}));
  REQUIRE(tableManager.getBySecondArgT(1) ==
      unordered_set<int>({2, 3, 4, 5, 6}));
}

TEST_CASE(
    "TransitiveRelationTableManager queryT known arg1 values with arg2 "
    "predicate") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(1, 2);
  tableManager.insert(1, 6);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 7);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.queryT({1, 3}, isValid);

  REQUIRE(res.firstArgVals == unordered_set<int>({1, 3}));
  REQUIRE(res.secondArgVals == unordered_set<int>({2, 4, 7}));
  REQUIRE(res.pairVals == pair_set<int, int>({{1, 2}, {1, 7}, {3, 4}}));
}

TEST_CASE(
    "TransitiveRelationTableManager queryT known arg2 values with arg1 "
    "predicate") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(2, 1);
  tableManager.insert(6, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(7, 5);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.queryT(isValid, {1, 3});

  REQUIRE(res.firstArgVals == unordered_set<int>({2, 4, 7}));
  REQUIRE(res.secondArgVals == unordered_set<int>({1, 3}));
  REQUIRE(res.pairVals == pair_set<int, int>({{2, 1}, {7, 1}, {4, 3}}));
}

TEST_CASE(
    "TransitiveRelationTableManager queryT known arg1 with arg2 predicate") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(1, 2);
  tableManager.insert(1, 6);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 7);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.queryT(1, isValid);

  REQUIRE(res.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res.secondArgVals == unordered_set<int>({2, 7}));
  REQUIRE(res.pairVals == pair_set<int, int>({{1, 2}, {1, 7}}));
}

TEST_CASE(
    "TransitiveRelationTableManager queryT known arg2 with arg1 predicate") {
  TransitiveRelationTableManager<int> tableManager(
      make_shared<ContiguousSetTable<int>>(),
      make_shared<ContiguousSetTable<int>>());

  tableManager.insert(2, 1);
  tableManager.insert(6, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(7, 5);

  unordered_set<int> validValues({2, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.queryT(isValid, 1);

  REQUIRE(res.firstArgVals == unordered_set<int>({2, 7}));
  REQUIRE(res.secondArgVals == unordered_set<int>({1}));
  REQUIRE(res.pairVals == pair_set<int, int>({{2, 1}, {7, 1}}));
}
