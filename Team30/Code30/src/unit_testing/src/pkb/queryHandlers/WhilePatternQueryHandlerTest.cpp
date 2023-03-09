#include <unordered_set>
#include <memory>
#include <string>

#include "catch.hpp"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/queryHandlers/WhilePatternQueryHandler.h"
#include "StructureMappingProviderStub.h"

using std::unordered_set, std::shared_ptr, std::unique_ptr,
    std::make_unique, std::make_shared, std::string;

static unique_ptr<StructureMappingProviderStub> setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(1, StmtType::While);
  provider->stmtNumToType.set(2, StmtType::While);
  provider->stmtNumToType.set(3, StmtType::While);
  provider->stmtNumToType.set(4, StmtType::While);
  provider->stmtNumToType.set(5, StmtType::If);

  provider->stmtTypeToNum.set(StmtType::While, 1);
  provider->stmtTypeToNum.set(StmtType::While, 2);
  provider->stmtTypeToNum.set(StmtType::While, 3);
  provider->stmtTypeToNum.set(StmtType::While, 4);
  provider->stmtTypeToNum.set(StmtType::If, 5);

  provider->allStmts = {1, 2, 3, 4, 5};
  return provider;
}

struct whilePatternTestInit {
  shared_ptr<HashKeySetTable<int, string>> table;
  shared_ptr<HashKeySetTable<string, int>> reverseTable;
  unique_ptr<WhilePatternStorage> store;
  unique_ptr<StructureMappingProviderStub> structureProvider;
  unique_ptr<PredicateFactory> factory;
  WhilePatternQueryHandler handler;

  whilePatternTestInit()
      : table(make_shared<HashKeySetTable<int, string>>()),
        reverseTable(make_shared<HashKeySetTable<string, int>>()),
        store(make_unique<WhilePatternStorage>(table, reverseTable)),
        structureProvider(setUpStructureMappingProvider()),
        factory(make_unique<PredicateFactory>(structureProvider.get(),
                                              nullptr)),
        handler(WhilePatternQueryHandler(store.get(),
                                         factory.get(),
                                         structureProvider.get())) {
    table->set(1, "a");
    table->set(2, "a");
    table->set(1, "b");
    table->set(3, "c");
    reverseTable->set("a", 1);
    reverseTable->set("a", 2);
    reverseTable->set("b", 1);
    reverseTable->set("c", 3);
  };
};

TEST_CASE("WhilePatternQueryHandler whiles(varname,_)") {
  auto test = whilePatternTestInit();

  // positive
  auto res1 =
      test.handler.queryWhilePattern({StmtType::While, 0},
                                     {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 =
      test.handler.queryWhilePattern({StmtType::While, 0},
                                     {EntityType::None, "c"});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // negative
  auto res3 =
      test.handler.queryWhilePattern({StmtType::While, 0},
                                     {EntityType::None, "f"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(v,_) or w(_,_)") {
  auto test = whilePatternTestInit();

  auto res1 =
      test.handler.queryWhilePattern({StmtType::While, 0},
                                     {EntityType::None, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  auto res2 = test.handler.queryWhilePattern({StmtType::While, 0},
                                             {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  // invalid arg2
  auto res3 = test.handler.queryWhilePattern({StmtType::While, 0},
                                             {EntityType::Procedure, ""});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(varname,_) with w.stmt# ") {
  auto test = whilePatternTestInit();

  // positive
  auto res1 =
      test.handler.queryWhilePattern({StmtType::None, 1},
                                     {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}}));

  // negative
  auto res3 =
      test.handler.queryWhilePattern({StmtType::None, 1},
                                     {EntityType::None, "c"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(v,_) / w(_,_) with w.stmt# ") {
  auto test = whilePatternTestInit();

  auto res1 =
      test.handler.queryWhilePattern({StmtType::None, 1},
                                     {EntityType::None, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a", "b"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}}));

  auto res2 = test.handler.queryWhilePattern({StmtType::None, 3},
                                             {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // invalid arg1
  auto res3 = test.handler.queryWhilePattern({StmtType::If, 1},
                                             {EntityType::Variable, ""});
  REQUIRE(res3.isEmpty == true);
}
