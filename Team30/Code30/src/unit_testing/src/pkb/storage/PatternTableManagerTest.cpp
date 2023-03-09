#include <string>
#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/storage/PatternTableManager.h"

using std::string, std::unordered_set, std::make_shared;

TEST_CASE("PatternTableManager insert") {
  auto table = make_shared<HashKeySetTable<string, int>>();
  PatternTableManager<int, string> tableManager(table);

  tableManager.insert(1, "a");
  tableManager.insert(1, "b");
  tableManager.insert(2, "a");

  auto res = tableManager.queryByValue("a");

  REQUIRE(table->get("a") == unordered_set<int>({1, 2}));
  REQUIRE(table->get("b") == unordered_set<int>({1}));
}

TEST_CASE("PatternTableManager queryByValue") {
  PatternTableManager<int, string>
      tableManager(make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(1, "b");
  tableManager.insert(2, "a");
  tableManager.insert(3, "c");

  auto res = tableManager.queryByValue("a");

  REQUIRE(res.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));
}

TEST_CASE("PatternTableManager queryAll") {
  PatternTableManager<int, string>
      tableManager(make_shared<HashKeySetTable<string, int>>());

  tableManager.insert(1, "a");
  tableManager.insert(1, "b");
  tableManager.insert(2, "a");
  tableManager.insert(3, "c");

  auto res = tableManager.queryAll();

  REQUIRE(res.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                 {3, "c"}}));
}
