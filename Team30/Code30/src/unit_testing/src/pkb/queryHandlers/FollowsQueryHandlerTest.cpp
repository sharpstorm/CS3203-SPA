#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"

using std::make_pair;
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
      default:
        return false;
    }
  }
  unordered_set<int> getStatementsOfType(StmtType stmtType) const override {
    switch (stmtType) {
      case StmtType::Assign:
        return unordered_set<int>({2, 3});
      case StmtType::Read:
        return unordered_set<int>({4, 5});
      default:
        return unordered_set<int>();
    }
  }
};

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
      handler.queryFollows({StmtType::None, 2}, {StmtType::None, 3}).isEmpty ==
      true);
  REQUIRE(
      handler.queryFollows({StmtType::None, 4}, {StmtType::None, 4}).isEmpty ==
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
  REQUIRE(result1.pairVals == unordered_set<pair<int, int>>({make_pair(1, 2)}));

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
  REQUIRE(result1.pairVals == unordered_set<pair<int, int>>({make_pair(2, 5)}));

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
  // REQUIRE(result1.pairVals ==
  //        unordered_set<pair<int, int>>({make_pair(2, 3), make_pair(4, 5)}));
}
