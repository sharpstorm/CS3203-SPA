#include <memory>

#include "EntityMappingProviderStub.h"
#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/IfPatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::shared_ptr, std::unique_ptr, std::make_unique, std::make_shared;

static unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.insert(1, StmtType::If);
  provider->stmtNumToType.insert(2, StmtType::If);
  provider->stmtNumToType.insert(3, StmtType::If);
  provider->stmtNumToType.insert(4, StmtType::If);
  provider->stmtNumToType.insert(5, StmtType::While);

  provider->stmtTypeToNum.insert(StmtType::If, 1);
  provider->stmtTypeToNum.insert(StmtType::If, 2);
  provider->stmtTypeToNum.insert(StmtType::If, 3);
  provider->stmtTypeToNum.insert(StmtType::If, 4);
  provider->stmtTypeToNum.insert(StmtType::While, 5);

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

struct ifPatternTest {
  shared_ptr<IfPatternTable> table = make_shared<IfPatternTable>();
  shared_ptr<IfPatternRevTable> reverseTable = make_shared<IfPatternRevTable>();
  unique_ptr<IfPatternStorage> store =
      make_unique<IfPatternStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      setUpStructureMappingProvider();
  unique_ptr<EntityMappingProviderStub> entityProvider =
      setUpEntityMappingProvider();
  unique_ptr<StmtPredicateFactory> stmtPredFactory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<EntityPredicateFactory> entPredFactory =
      make_unique<EntityPredicateFactory>();
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntInvoker =
      make_unique<PkbStmtEntQueryInvoker>(
          structureProvider.get(), entityProvider.get(), stmtPredFactory.get(),
          entPredFactory.get());
  IfPatternQueryHandler handler =
      IfPatternQueryHandler(stmtEntInvoker.get(), store.get());
  ifPatternTest() {
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

TEST_CASE("IfPatternQueryHandler ifs(varname,_,_)") {
  auto test = ifPatternTest();

  // positive
  auto res1 = *test.query({StmtType::If, 0}, {EntityType::None, "a"});
  REQUIRE(res1.getLeftVals() == StmtValueSet({1, 2}));

  auto res2 = *test.query({StmtType::If, 0}, {EntityType::None, "c"});
  REQUIRE(res2.getLeftVals() == StmtValueSet({3}));

  // negative
  auto res3 = *test.query({StmtType::If, 0}, {EntityType::None, "f"});
  REQUIRE(res3.empty() == true);
}

TEST_CASE("IfPatternQueryHandler ifs(v,_,_) or ifs(_,_,_)") {
  auto test = ifPatternTest();

  auto res1 = *test.query({StmtType::If, 0}, {EntityType::Wildcard, ""});
  REQUIRE(res1.getLeftVals() == StmtValueSet({1, 2, 3}));

  auto res2 = *test.query({StmtType::If, 0}, {EntityType::Variable, ""});
  REQUIRE(res2.getPairVals() == pair_set<StmtValue, EntityValue>(
                                    {{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  // invalid arg2
  auto res3 = *test.query({StmtType::If, 0}, {EntityType::Procedure, ""});
  REQUIRE(res3.empty() == true);
}

TEST_CASE("IfPatternQueryHandler ifs(varname,_,_) with ifs.stmt# ") {
  auto test = ifPatternTest();

  // positive
  auto res1 = *test.query({StmtType::None, 1}, {EntityType::None, "a"});
  REQUIRE(res1.empty() == false);

  // negative
  auto res3 = *test.query({StmtType::None, 1}, {EntityType::None, "c"});
  REQUIRE(res3.empty() == true);
}

TEST_CASE("IfPatternQueryHandler ifs(v,_,_) / ifs(_,_,_) with ifs.stmt# ") {
  auto test = ifPatternTest();

  auto res1 = *test.query({StmtType::None, 1}, {EntityType::Wildcard, ""});
  REQUIRE(res1.empty() == false);

  auto res2 = *test.query({StmtType::None, 3}, {EntityType::Variable, ""});
  REQUIRE(res2.getRightVals() == EntityValueSet({"c"}));

  // invalid arg1
  auto res3 = *test.query({StmtType::While, 1}, {EntityType::Variable, ""});
  REQUIRE(res3.empty() == true);
}
