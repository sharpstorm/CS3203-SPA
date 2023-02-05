#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"

using std::pair;
using std::unordered_map;
using std::unordered_set;

class StructureMappingProviderStub : public StructureMappingProvider {
 public:
  StructureMappingProviderStub(){};

  bool isStatementOfType(int s, StmtType stmtType) const override {
    switch (s) {
      case 2:
      case 3:
        return stmtType == StmtType::Assign;
      case 4:
      case 5:
        return stmtType == StmtType::Read;
      case 11:
      case 14:
        return stmtType == StmtType::If;
      case 12:
      case 16:
        return stmtType == StmtType::While;
      default:
        return false;
    }
  }
  unordered_set<int> getStatementsOfType(StmtType stmtType) const override {
    switch (stmtType) {
      case StmtType::Assign:
        return {2, 3};
      case StmtType::Read:
        return {4, 5};
      case StmtType::If:
        return {11, 14};
      case StmtType::While:
        return {12, 16};
      default:
        return unordered_set<int>();
    }
  }
};

/* Follows */

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtNum)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(1, 2);
  pkb.followsStore->insert(2, 4);

  REQUIRE(
      handler.queryFollows({StmtType::None, 1}, {StmtType::None, 2}).isEmpty ==
      false);
  REQUIRE(
      handler.queryFollows({StmtType::None, 2}, {StmtType::None, 1}).isEmpty ==
      true);
  REQUIRE(
      handler.queryFollows({StmtType::None, 4}, {StmtType::None, 4}).isEmpty ==
      true);
  REQUIRE(
      handler.queryFollows({StmtType::None, 1}, {StmtType::None, 1}).isEmpty ==
      true);
}

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtType)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(1, 2);
  pkb.followsStore->insert(2, 3);
  pkb.followsStore->insert(3, 5);

  auto result1 =
      handler.queryFollows({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = handler.queryFollows({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtNum)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(2, 5);
  pkb.followsStore->insert(5, 6);
  pkb.followsStore->insert(6, 8);

  auto result1 =
      handler.queryFollows({StmtType::Assign, 0}, {StmtType::None, 5});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}}));

  auto result2 = handler.queryFollows({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtType)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(2, 5);
  pkb.followsStore->insert(3, 4);
  pkb.followsStore->insert(5, 6);

  auto result1 =
      handler.queryFollows({StmtType::Assign, 0}, {StmtType::Read, 0});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5, 4}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}, {3, 4}}));
}

/* FollowsStar */
TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtNum)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(1, 2);
  pkb.followsStore->insert(2, 5);

  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 2})
              .isEmpty == false);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 5})
              .isEmpty == false);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 1})
              .isEmpty == true);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 5}, {StmtType::None, 2})
              .isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtType)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(10, 11);
  pkb.followsStore->insert(11, 12);
  pkb.followsStore->insert(12, 16);

  auto result1 =
      handler.queryFollowsStar({StmtType::None, 10}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({10}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{10, 12}, {10, 16}}));

  auto result2 =
      handler.queryFollowsStar({StmtType::None, 12}, {StmtType::If, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtNum)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(11, 12);
  pkb.followsStore->insert(12, 13);
  pkb.followsStore->insert(13, 14);

  auto result1 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::None, 13});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({13}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 13}}));

  auto result2 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::None, 11});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtType)") {
  PKB pkb = PKB();
  StructureMappingProviderStub* provider = new StructureMappingProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler handler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  pkb.followsStore->insert(11, 12);
  pkb.followsStore->insert(12, 14);
  pkb.followsStore->insert(14, 15);
  pkb.followsStore->insert(15, 16);

  auto result1 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11, 14}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals ==
          pair_set<int, int>({{11, 12}, {11, 16}, {14, 16}}));
}
