
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_shared;
using std::pair;
using std::set;
using std::string;
using std::unique_ptr;
TEST_CASE("RelationTableManager insert and getByArg1, getByArg2") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(table.get(),
                                                 reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "b");

  REQUIRE(tableManager.getByFirstArg(1) == set<string>({"a", "c"}));
  REQUIRE(tableManager.getBySecondArg("b") == set<int>({2, 4}));
}

TEST_CASE("RelationTableManager query known arg1 values with arg2 predicate") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(table.get(),
                                                 reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "e");
  tableManager.insert(3, "f");

  unordered_set<string> validValues({"a", "e", "f"});
  Predicate<string> isValid = [validValues](const string s) -> bool {
    return validValues.find(s) != validValues.end();
  };

  auto resultBuilder = QueryResultBuilder<int, string>();
  resultBuilder.setAllVals();
  auto res = tableManager.query({1, 2, 4}, isValid, &resultBuilder);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({1, 4}));
  REQUIRE(res.get()->secondArgVals == unordered_set<string>({"a", "e"}));
  REQUIRE(res.get()->pairVals == pair_set<int, string>({{1, "a"}, {4, "e"}}));
}

TEST_CASE("RelationTableManager query known arg2 values with arg1 predicate") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(table.get(),
                                                 reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");
  tableManager.insert(2, "c");
  tableManager.insert(3, "f");

  unordered_set<int> validValues({2, 4, 5});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto resultBuilder = QueryResultBuilder<int, string>();
  resultBuilder.setAllVals();
  auto res = tableManager.query(isValid, {"a", "b", "f"}, &resultBuilder);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({2, 4}));
  REQUIRE(res.get()->secondArgVals == unordered_set<string>({"a", "b"}));
  REQUIRE(res.get()->pairVals == pair_set<int, string>({{4, "a"}, {2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg1 with arg2 predicate") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(table.get(),
                                                 reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(2, "c");

  unordered_set<string> validValues({"a", "b"});
  Predicate<string> isValid = [validValues](string const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto resultBuilder = QueryResultBuilder<int, string>();
  resultBuilder.setAllVals();
  auto res = tableManager.query(2, isValid, &resultBuilder);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({2}));
  REQUIRE(res.get()->secondArgVals == unordered_set<string>({"b"}));
  REQUIRE(res.get()->pairVals == pair_set<int, string>({{2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg2 with arg1 predicate") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(table.get(),
                                                 reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");

  unordered_set<int> validValues({2, 4});
  Predicate<int> isValid = [validValues](int const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto resultBuilder = QueryResultBuilder<int, string>();
  resultBuilder.setAllVals();
  auto res = tableManager.query(isValid, "a", &resultBuilder);

  REQUIRE(res.get()->firstArgVals == unordered_set<int>({4}));
  REQUIRE(res.get()->secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res.get()->pairVals == unordered_set<pair<int, string>>({{4, "a"}}));
}

TEST_CASE("RelationTableManager query both known args") {
  auto table = make_shared<ContiguousSetTable<string>>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  RelationTableManager<int, string> tableManager(
      table.get(),
      reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(1, "b");
  tableManager.insert(1, "c");
  tableManager.insert(2, "a");

  auto res1 = tableManager.query(1, "b");
  REQUIRE(res1.get()->isEmpty == false);
  REQUIRE(res1.get()->pairVals == unordered_set<pair<int, string>>({{1, "b"}}));

  auto res2 = tableManager.query(2, "b");
  REQUIRE(res2.get()->isEmpty == true);

  auto res3 = tableManager.query(3, "a");
  REQUIRE(res3.get()->isEmpty == true);
}
