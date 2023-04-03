#include <memory>
#include <string>
#include <unordered_set>

#include "EntityMappingProviderStub.h"
#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "pkb/queryHandlers/WhilePatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::unordered_set, std::shared_ptr, std::unique_ptr, std::make_unique,
    std::make_shared, std::string;

static unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.insert(1, StmtType::While);
  provider->stmtNumToType.insert(2, StmtType::While);
  provider->stmtNumToType.insert(3, StmtType::While);
  provider->stmtNumToType.insert(4, StmtType::While);
  provider->stmtNumToType.insert(5, StmtType::If);

  provider->stmtTypeToNum.insert(StmtType::While, 1);
  provider->stmtTypeToNum.insert(StmtType::While, 2);
  provider->stmtTypeToNum.insert(StmtType::While, 3);
  provider->stmtTypeToNum.insert(StmtType::While, 4);
  provider->stmtTypeToNum.insert(StmtType::If, 5);

  provider->allStmts = {1, 2, 3, 4, 5};
  return provider;
}

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->variableTable.insert(1, "a");
  provider->variableTable.insert(2, "b");
  provider->variableTable.insert(3, "c");
  provider->allVariables = {"a", "b", "c"};
  return provider;
}

struct whilePatternTest {
  shared_ptr<WhilePatternTable> table = make_shared<WhilePatternTable>();
  shared_ptr<WhilePatternRevTable> reverseTable =
      make_shared<WhilePatternRevTable>();
  unique_ptr<WhilePatternStorage> store =
      make_unique<WhilePatternStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      setUpStructureMappingProvider();
  unique_ptr<StmtPredicateFactory> stmtPredFactory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<EntityPredicateFactory> entPredFactory =
      make_unique<EntityPredicateFactory>();
  unique_ptr<EntityMappingProviderStub> entityProvider =
      setUpEntityMappingProvider();
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntInvoker =
      make_unique<PkbStmtEntQueryInvoker>(
          structureProvider.get(), entityProvider.get(), stmtPredFactory.get(),
          entPredFactory.get());
  WhilePatternQueryHandler handler =
      WhilePatternQueryHandler(stmtEntInvoker.get(), store.get());
  whilePatternTest() {
    table->insert(1, "a");
    table->insert(2, "a");
    table->insert(1, "b");
    table->insert(3, "c");
    reverseTable->insert("a", 1);
    reverseTable->insert("a", 2);
    reverseTable->insert("b", 1);
    reverseTable->insert("c", 3);
  }

  QueryResultPtr<StmtValue, EntityValue> query(StmtRef leftArg,
                                               EntityRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }
};

TEST_CASE("WhilePatternQueryHandler whiles(varname,_)") {
  auto test = whilePatternTest();

  // positive
  auto res1 = *test.query({StmtType::While, 0}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
//  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
//  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 = *test.query({StmtType::While, 0}, {EntityType::None, "c"});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
//  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
//  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // negative
  auto res3 = *test.query({StmtType::While, 0}, {EntityType::None, "f"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(v,_) or w(_,_)") {
  auto test = whilePatternTest();

  auto res1 = *test.query({StmtType::While, 0}, {EntityType::Wildcard, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2, 3}));

  auto res2 = *test.query({StmtType::While, 0}, {EntityType::Variable, ""});
//  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
//  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals ==
          pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  // invalid arg2
  auto res3 = *test.query({StmtType::While, 0}, {EntityType::Procedure, ""});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(varname,_) with w.stmt# ") {
  auto test = whilePatternTest();

  // positive
  auto res1 = *test.query({StmtType::None, 1}, {EntityType::None, "a"});
//  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
//  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}}));

  // negative
  auto res3 = *test.query({StmtType::None, 1}, {EntityType::None, "c"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("WhilePatternQueryHandler w(v,_) / w(_,_) with w.stmt# ") {
  auto test = whilePatternTest();

  auto res1 = *test.query({StmtType::None, 1}, {EntityType::Wildcard, ""});
  REQUIRE(res1.isEmpty == false);

  auto res2 = *test.query({StmtType::None, 3}, {EntityType::Variable, ""});
//  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
//  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // invalid arg1
  auto res3 = *test.query({StmtType::If, 1}, {EntityType::Variable, ""});
  REQUIRE(res3.isEmpty == true);
}
