#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/FollowsWriter.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set;

class FollowsTestStructureProviderStub : public StructureMappingProvider {
 public:
  FollowsTestStructureProviderStub()
      : StructureMappingProvider(new StatementStorage(),
                                 new ProcedureStorage()) {};

  bool isStatementOfType(int s, StmtType stmtType) const override {
    switch (s) {
      case 5:
      case 7:return stmtType == StmtType::Assign;
      default:return false;
    }
  }
  unordered_set<int> getStatementsOfType(StmtType stmtType) const override {
    switch (stmtType) {
      case StmtType::Assign:return {5, 7};
      default:return unordered_set<int>();
    }
  }
};

TEST_CASE("Follows with StructureMapProvider stub") {
  PKB pkb = PKB();
  FollowsTestStructureProviderStub *provider =
      new FollowsTestStructureProviderStub();
  PredicateFactory *factory = new PredicateFactory(provider, nullptr);
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

TEST_CASE("FollowsStar with StructureMapProvider stub") {
  PKB pkb = PKB();
  FollowsTestStructureProviderStub *provider =
      new FollowsTestStructureProviderStub();
  PredicateFactory *factory = new PredicateFactory(provider, nullptr);
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

TEST_CASE("Follows") {
  PKB *pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);
  REQUIRE(queryHandler.queryFollows({StmtType::None, 1}, {StmtType::None, 2})
              .isEmpty == false);
  REQUIRE(queryHandler.queryFollows({StmtType::None, 1}, {StmtType::None, 3})
              .isEmpty == true);
  auto result1 =
      queryHandler.queryFollows({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 =
      queryHandler.queryFollows({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{3, 4}}));

  auto result3 =
      queryHandler.queryFollows({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.pairVals == pair_set<int, int>({{3, 4}}));

  auto result4 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::None, 4});
  REQUIRE(result4.isEmpty == false);
  REQUIRE(result4.pairVals == pair_set<int, int>({{3, 4}}));

}

TEST_CASE("Follows follows(s,stmtType) or follows(s,s)") {
  PKB *pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}}));

  auto result2 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}}));

}

TEST_CASE("FollowsStar") {
  PKB *pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);
  writer.addFollows(4, 5);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::If);

  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);
  REQUIRE(
      queryHandler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 4})
          .isEmpty == false);
  REQUIRE(
      queryHandler.queryFollowsStar({StmtType::None, 3}, {StmtType::None, 1})
          .isEmpty == true);
  auto result1 =
      queryHandler.queryFollowsStar({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {1, 3}}));

  auto result2 =
      queryHandler.queryFollowsStar({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));

  auto result3 =
      queryHandler.queryFollowsStar({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));
}

TEST_CASE("FollowsStar followsStar(s,stmtType) or followsStar(s,s)") {
  PKB *pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      queryHandler.queryFollowsStar({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {1, 3}}));

  auto result2 =
      queryHandler.queryFollowsStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals
              == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}, {1, 3}, {1, 4},
                                     {2, 4}}));

}
