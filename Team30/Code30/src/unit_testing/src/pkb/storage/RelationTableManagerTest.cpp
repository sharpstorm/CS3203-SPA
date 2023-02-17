
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_shared;
using std::pair;
using std::string;
using std::unordered_set;

TEST_CASE("RelationTableManager insert and getByArg1, getByArg2") {
  RelationTableManager<int, string> tableManager(
      make_shared<ContiguousSetTable<string>>(),
      make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "b");

  REQUIRE(tableManager.getByFirstArg(1) == unordered_set<string>({"a", "c"}));
  REQUIRE(tableManager.getBySecondArg("b") == unordered_set<int>({2, 4}));
}

TEST_CASE("RelationTableManager query known arg1 values with arg2 predicate") {
  RelationTableManager<int, string> tableManager(
      make_shared<ContiguousSetTable<string>>(),
      make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "e");
  tableManager.insert(3, "f");

  unordered_set<string> validValues({"a", "e", "f"});
  Predicate<string> isValid = [validValues](string const &s) {
    return validValues.find(s) != validValues.end();
  };

  auto res = tableManager.query({1, 2, 4}, isValid);

  REQUIRE(res.firstArgVals == unordered_set<int>({1, 4}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"a", "e"}));
  REQUIRE(res.pairVals == pair_set<int, string>({{1, "a"}, {4, "e"}}));
}

TEST_CASE("RelationTableManager query known arg2 values with arg1 predicate") {
  RelationTableManager<int, string> tableManager(
      make_shared<ContiguousSetTable<string>>(),
      make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");
  tableManager.insert(2, "c");
  tableManager.insert(3, "f");

  unordered_set<int> validValues({2, 4, 5});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };

  auto res = tableManager.query(isValid, {"a", "b", "f"});

  REQUIRE(res.firstArgVals == unordered_set<int>({2, 4}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"a", "b"}));
  REQUIRE(res.pairVals == pair_set<int, string>({{4, "a"}, {2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg1 with arg2 predicate") {
  RelationTableManager<int, string> tableManager(
      make_shared<ContiguousSetTable<string>>(),
      make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(2, "c");

  unordered_set<string> validValues({"a", "b"});
  Predicate<string> isValid = [validValues](string const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query(2, isValid);

  REQUIRE(res.firstArgVals == unordered_set<int>({2}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"b"}));
  REQUIRE(res.pairVals == pair_set<int, string>({{2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg2 with arg1 predicate") {
  RelationTableManager<int, string> tableManager(
      make_shared<ContiguousSetTable<string>>(),
      make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");

  unordered_set<int> validValues({2, 4});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto res = tableManager.query(isValid, "a");

  REQUIRE(res.firstArgVals == unordered_set<int>({4}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res.pairVals == unordered_set<pair<int, string>>({{4, "a"}}));
}
