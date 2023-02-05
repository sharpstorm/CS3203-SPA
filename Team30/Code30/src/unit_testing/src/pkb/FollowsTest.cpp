#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/FollowsWriter.h"

using std::unordered_set;

class FollowsTestStructureProviderStub : public StructureMappingProvider {
 public:
  FollowsTestStructureProviderStub(){};

  bool isStatementOfType(int s, StmtType stmtType) const override {
    switch (s) {
      case 5:
      case 7:
        return stmtType == StmtType::Assign;
      default:
        return false;
    }
  }
  unordered_set<int> getStatementsOfType(StmtType stmtType) const override {
    switch (stmtType) {
      case StmtType::Assign:
        return {5, 7};
      default:
        return unordered_set<int>();
    }
  }
};

TEST_CASE("Follows") {
  PKB pkb = PKB();
  FollowsTestStructureProviderStub* provider =
      new FollowsTestStructureProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler queryHandler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  FollowsWriter writer = FollowsWriter(pkb.followsStore);

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 5);

  auto result1 =
      queryHandler.queryFollows({StmtType::None, 3}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{3, 5}}));
  auto result2 =
      queryHandler.queryFollows({StmtType::Assign, 0}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsStar") {
  PKB pkb = PKB();
  FollowsTestStructureProviderStub* provider =
      new FollowsTestStructureProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  FollowsQueryHandler queryHandler =
      FollowsQueryHandler(pkb.followsStore, factory, provider);
  FollowsWriter writer = FollowsWriter(pkb.followsStore);

  writer.addFollows(1, 3);
  writer.addFollows(3, 5);
  writer.addFollows(5, 6);
  writer.addFollows(6, 7);
  writer.addFollows(7, 8);

  auto result1 =
      queryHandler.queryFollowsStar({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 5}, {1, 7}}));
}
