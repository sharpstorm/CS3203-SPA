
#include <memory>
#include <string>
#include <set>
#include <utility>

#include "catch.hpp"
#include "pkb/PkbTypes.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/TransitiveRelationTableManager.h"
#include "pkb/storage/tables/ContiguousSetTable.h"

using std::make_pair;
using std::make_shared;
using std::string;
using std::set;

TEST_CASE("TransitiveRelationTableManager getByFirstArg") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 2);
  tableManager.insert(1, 3);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 6);

  REQUIRE(tableManager.getByFirstArg(6) == set<int>({}));
  REQUIRE(tableManager.getByFirstArg(5) == set<int>({6}));
  REQUIRE(tableManager.getByFirstArg(1) ==
      set<int>({2, 3, 4, 5, 6}));
}

TEST_CASE("TransitiveRelationTableManager getBySecondArg") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(2, 1);
  tableManager.insert(3, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(6, 5);

  REQUIRE(tableManager.getBySecondArg(6) == set<int>({}));
  REQUIRE(tableManager.getBySecondArg(5) == set<int>({6}));
  REQUIRE(tableManager.getBySecondArg(1) ==
      set<int>({2, 3, 4, 5, 6}));
}

TEST_CASE(
    "TransitiveRelationTableManager query known arg1 values with arg2 "
    "predicate") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();

  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 2);
  tableManager.insert(1, 6);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 7);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query({1, 3}, isValid);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({1, 3}));
  REQUIRE(res.get()->secondArgVals == unordered_set<int>({2, 4, 7}));
  REQUIRE(res.get()->pairVals == pair_set<int, int>({{1, 2}, {1, 7}, {3, 4}}));
}

TEST_CASE(
    "TransitiveRelationTableManager query known arg2 values with arg1 "
    "predicate") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(2, 1);
  tableManager.insert(6, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(7, 5);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query(isValid, {1, 3});

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({2, 4, 7}));
  REQUIRE(res.get()->secondArgVals == unordered_set<int>({1, 3}));
  REQUIRE(res.get()->pairVals == pair_set<int, int>({{2, 1}, {7, 1}, {4, 3}}));
}

TEST_CASE(
    "TransitiveRelationTableManager query known arg1 with arg2 predicate") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 2);
  tableManager.insert(1, 6);
  tableManager.insert(2, 5);
  tableManager.insert(3, 4);
  tableManager.insert(5, 7);

  unordered_set<int> validValues({2, 4, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query(1, isValid);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({1}));
  REQUIRE(res.get()->secondArgVals == unordered_set<int>({2, 7}));
  REQUIRE(res.get()->pairVals == pair_set<int, int>({{1, 2}, {1, 7}}));
}

TEST_CASE(
    "TransitiveRelationTableManager query known arg2 with arg1 predicate") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(2, 1);
  tableManager.insert(6, 1);
  tableManager.insert(5, 2);
  tableManager.insert(4, 3);
  tableManager.insert(7, 5);

  unordered_set<int> validValues({2, 7});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query(isValid, 1);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({2, 7}));
  REQUIRE(res.get()->secondArgVals == unordered_set<int>({1}));
  REQUIRE(res.get()->pairVals == pair_set<int, int>({{2, 1}, {7, 1}}));
}

TEST_CASE("TransitiveRelationTableManager cyclic direct") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 1);

  REQUIRE(tableManager.getByFirstArg(1) == set<int>({1}));
}

TEST_CASE("TransitiveRelationTableManager cyclic indirect 1") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 2);
  tableManager.insert(2, 1);

  REQUIRE(tableManager.getByFirstArg(1) == set<int>({1, 2}));
}

TEST_CASE("TransitiveRelationTableManager cyclic indirect 2") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  TransitiveRelationTableManager<int> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, 2);
  tableManager.insert(2, 3);
  tableManager.insert(3, 1);

  REQUIRE(tableManager.getByFirstArg(1) == set<int>({1, 2, 3}));
}

TEST_CASE("TransitiveRelationTableManager called as parent class type") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  unique_ptr<RelationTableManager<int, int>>
      tableManager = make_unique<TransitiveRelationTableManager<int>>(
      table.get(),
      reverseTable.get());

  tableManager->insert(1, 2);
  tableManager->insert(2, 3);

  Predicate<int> isValid = [](int const &s) {
    return true;
  };
  REQUIRE(tableManager->getByFirstArg(1) == set<int>({2, 3}));
  REQUIRE(tableManager->getBySecondArg(3) == set<int>({1, 2}));
  auto res = tableManager->query(1, isValid);
  REQUIRE(res.get()->pairVals == pair_set<int, int>({{1, 2}, {1, 3}}));
}
