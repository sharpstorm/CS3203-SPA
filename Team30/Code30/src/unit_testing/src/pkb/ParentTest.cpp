#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/ParentWriter.h"

using std::unordered_set;

class ParentTestStructureProviderStub : public StructureMappingProvider {
 public:
  ParentTestStructureProviderStub(){};

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

TEST_CASE("Parent") {
  PKB pkb = PKB();
  ParentTestStructureProviderStub* provider =
      new ParentTestStructureProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  ParentQueryHandler queryHandler =
      ParentQueryHandler(pkb.parentStore, factory, provider);
  ParentWriter writer = ParentWriter(pkb.parentStore);

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(3, 5);

  auto result1 =
      queryHandler.queryParent({StmtType::None, 3}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{3, 5}}));
  auto result2 =
      queryHandler.queryParent({StmtType::Assign, 0}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentStar") {
  PKB pkb = PKB();
  ParentTestStructureProviderStub* provider =
      new ParentTestStructureProviderStub();
  PredicateFactory* factory = new PredicateFactory(provider);
  ParentQueryHandler queryHandler =
      ParentQueryHandler(pkb.parentStore, factory, provider);
  ParentWriter writer = ParentWriter(pkb.parentStore);

  writer.addParent(1, 3);
  writer.addParent(3, 5);
  writer.addParent(5, 6);
  writer.addParent(6, 7);
  writer.addParent(7, 8);

  auto result1 =
      queryHandler.queryParentStar({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 5}, {1, 7}}));
}
