#include <memory>
#include <string>
#include <unordered_set>

#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "pkb/queryHandlers/IfPatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::unordered_set, std::shared_ptr, std::unique_ptr, std::make_unique,
    std::make_shared, std::string;

static unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(1, StmtType::If);
  provider->stmtNumToType.set(2, StmtType::If);
  provider->stmtNumToType.set(3, StmtType::If);
  provider->stmtNumToType.set(4, StmtType::If);
  provider->stmtNumToType.set(5, StmtType::While);

  provider->stmtTypeToNum.set(StmtType::If, 1);
  provider->stmtTypeToNum.set(StmtType::If, 2);
  provider->stmtTypeToNum.set(StmtType::If, 3);
  provider->stmtTypeToNum.set(StmtType::If, 4);
  provider->stmtTypeToNum.set(StmtType::While, 5);

  provider->allStmts = {1, 2, 3, 4, 5};
  return provider;
}

struct ifPatternTest {
  shared_ptr<IfPatternTable> table = make_shared<IfPatternTable>();
  shared_ptr<IfPatternRevTable> reverseTable = make_shared<IfPatternRevTable>();
  unique_ptr<IfPatternStorage> store =
      make_unique<IfPatternStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      setUpStructureMappingProvider();
  unique_ptr<StmtPredicateFactory> stmtPredFactory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<EntityPredicateFactory> entPredFactory =
      make_unique<EntityPredicateFactory>();
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntInvoker =
      make_unique<PkbStmtEntQueryInvoker>(
          structureProvider.get(), stmtPredFactory.get(), entPredFactory.get());
  IfPatternQueryHandler handler =
      IfPatternQueryHandler(stmtEntInvoker.get(), store.get());
  ifPatternTest() {
    table->set(1, "a");
    table->set(2, "a");
    table->set(1, "b");
    table->set(3, "c");
    reverseTable->set("a", 1);
    reverseTable->set("a", 2);
    reverseTable->set("b", 1);
    reverseTable->set("c", 3);
  }

  QueryResultPtr<StmtValue, EntityValue> query(StmtRef leftArg,
                                                        EntityRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }
};

TEST_CASE("IfPatternQueryHandler ifs(varname,_,_)") {
  auto test = ifPatternTest();

  // positive
  auto res1 = *test.query({StmtType::If, 0}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 = *test.query({StmtType::If, 0}, {EntityType::None, "c"});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // negative
  auto res3 = *test.query({StmtType::If, 0}, {EntityType::None, "f"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("IfPatternQueryHandler ifs(v,_,_) or ifs(_,_,_)") {
  auto test = ifPatternTest();

  auto res1 = *test.query({StmtType::If, 0}, {EntityType::None, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res1.pairVals ==
          pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  auto res2 = *test.query({StmtType::If, 0}, {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals ==
          pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  // invalid arg2
  auto res3 = *test.query({StmtType::If, 0}, {EntityType::Procedure, ""});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("IfPatternQueryHandler ifs(varname,_,_) with ifs.stmt# ") {
  auto test = ifPatternTest();

  // positive
  auto res1 = *test.query({StmtType::None, 1}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}}));

  // negative
  auto res3 = *test.query({StmtType::None, 1}, {EntityType::None, "c"});
  REQUIRE(res3.isEmpty == true);
}

TEST_CASE("IfPatternQueryHandler ifs(v,_,_) / ifs(_,_,_) with ifs.stmt# ") {
  auto test = ifPatternTest();

  auto res1 = *test.query({StmtType::None, 1}, {EntityType::None, ""});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a", "b"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}}));

  auto res2 = *test.query({StmtType::None, 3}, {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{3, "c"}}));

  // invalid arg1
  auto res3 = *test.query({StmtType::While, 1}, {EntityType::Variable, ""});
  REQUIRE(res3.isEmpty == true);
}
