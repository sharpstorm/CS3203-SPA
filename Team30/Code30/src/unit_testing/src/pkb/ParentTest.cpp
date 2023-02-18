#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::unordered_set;
using std::make_unique;

TEST_CASE("Parent <= 1 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(3, 5);

  writer.addStatement(1, StmtType::If);
  writer.addStatement(2, StmtType::While);
  writer.addStatement(3, StmtType::While);
  writer.addStatement(5, StmtType::Assign);

  auto result1 = handler.queryParent({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(result1.isEmpty == false);

  auto result2 = handler.queryParent({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);
  auto result3 =
      handler.queryParent({StmtType::None, 3}, {StmtType::Assign, 0});
  REQUIRE(result3.pairVals == pair_set<int, int>({{3, 5}}));

  auto result4 =
      handler.queryParent({StmtType::While, 0}, {StmtType::None, 3});
  REQUIRE(result4.pairVals == pair_set<int, int>({{2, 3}}));

}

TEST_CASE("Parent 2 unknowns)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(3, 5);

  writer.addStatement(1, StmtType::If);
  writer.addStatement(2, StmtType::While);
  writer.addStatement(3, StmtType::While);
  writer.addStatement(5, StmtType::Assign);

  auto result1 =
      handler.queryParent({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 =
      handler.queryParent({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {3, 5}}));
}

TEST_CASE("ParentStar <= 1 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(2, 4);
  writer.addParent(2, 5);
  writer.addParent(6, 7);
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::Call);
  writer.addStatement(6, StmtType::While);
  writer.addStatement(7, StmtType::Print);

  auto result1 =
      handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 7});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      handler.queryParentStar({StmtType::None, 1}, {StmtType::Print, 0});
  REQUIRE(result3.pairVals == pair_set<int, int>({{1, 3}, {1, 4}}));

  auto result4 =
      handler.queryParentStar({StmtType::While, 0}, {StmtType::None, 7});
  REQUIRE(result4.pairVals == pair_set<int, int>({{6, 7}}));
}

TEST_CASE("ParentStar 2 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(2, 4);
  writer.addParent(2, 5);
  writer.addParent(6, 7);
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::Call);
  writer.addStatement(6, StmtType::While);
  writer.addStatement(7, StmtType::Print);

  auto result1 =
      handler.queryParentStar({StmtType::While, 0}, {StmtType::Print, 0});
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 3}, {1, 4}, {6, 7}}));

  auto result2 =
      handler.queryParentStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals
              == pair_set<int, int>({{1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3},
                                     {2, 4}, {2, 5}, {6, 7}}));
}
