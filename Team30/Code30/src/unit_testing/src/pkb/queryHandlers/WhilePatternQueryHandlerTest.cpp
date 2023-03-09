#include <unordered_set>
#include <memory>
#include <string>

#include "catch.hpp"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/queryHandlers/WhilePatternQueryHandler.h"

using std::unordered_set, std::shared_ptr, std::unique_ptr,
    std::make_unique, std::make_shared, std::string;

struct WhilePatternTestInit {
  shared_ptr<HashKeySetTable<string, int>> table;
  unique_ptr<WhilePatternStorage> store;
  WhilePatternQueryHandler handler;

  WhilePatternTestInit()
      : table(make_shared<HashKeySetTable<string, int>>()),
        store(make_unique<WhilePatternStorage>(table)),
        handler(WhilePatternQueryHandler(store.get())) {
    table->set("a", 1);
    table->set("a", 2);
    table->set("b", 1);
    table->set("c", 3);
  };
};

TEST_CASE("WhilePatternQueryHandler Whiles(var,_,_)") {
  auto test = WhilePatternTestInit();

  // positive
  auto res1 = test.handler.queryWhilePattern({EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 = test.handler.queryWhilePattern({EntityType::None, "c"});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // negative
  auto res3 = test.handler.queryWhilePattern({EntityType::None, "f"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler Whiles(v,_,_) or Whiles(_,_,_)") {
  auto test = WhilePatternTestInit();

  auto res1 = test.handler.queryWhilePattern({EntityType::None, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  auto res2 = test.handler.queryWhilePattern({EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  auto res3 = test.handler.queryWhilePattern({EntityType::Procedure, ""});
  REQUIRE(res3.isEmpty == true);
}
