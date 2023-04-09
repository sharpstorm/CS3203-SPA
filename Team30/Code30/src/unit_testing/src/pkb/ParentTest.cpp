#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;
using std::unordered_set;

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
  writer.runPostProcessor();

  auto result1 = *handler.queryParent({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(result1.isEmpty == false);

  auto result2 = *handler.queryParent({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);
  auto result3 =
      *handler.queryParent({StmtType::None, 3}, {StmtType::Assign, 0});
  REQUIRE(result3.secondArgVals == unordered_set<int>({5}));
  //  REQUIRE(result3.pairVals == pair_set<int, int>({{3, 5}}));

  auto result4 =
      *handler.queryParent({StmtType::While, 0}, {StmtType::None, 3});
  REQUIRE(result4.firstArgVals == unordered_set<int>({2}));
  //  REQUIRE(result4.pairVals == pair_set<int, int>({{2, 3}}));
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
  writer.runPostProcessor();

  auto result1 = *handler.queryParent({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = *handler.queryParent({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {3, 5}}));

  // wildcard
  auto result3 =
      *handler.queryParent({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result3.secondArgVals == unordered_set<int>({2, 3, 5}));

  auto result4 =
      *handler.queryParent({StmtType::Wildcard, 0}, {StmtType::None, 5});
  REQUIRE(result4.isEmpty == false);

  auto result5 =
      *handler.queryParent({StmtType::Wildcard, 0}, {StmtType::While, 0});
  REQUIRE(result5.secondArgVals == unordered_set<int>({2, 3}));
}

TEST_CASE("ParentStar <= 1 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  // 1(->2(->3|4),5,6-(>7))
  writer.addParent(1, 2);
  writer.addParent(1, 5);
  writer.addParent(1, 6);
  writer.addParent(2, 3);
  writer.addParent(2, 4);
  writer.addParent(6, 7);
  writer.addFollows(2, 5);
  writer.addFollows(2, 6);
  writer.addFollows(5, 6);
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::Read);
  writer.addStatement(6, StmtType::While);
  writer.addStatement(7, StmtType::Print);
  writer.runPostProcessor();

  auto result1 =
      *handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      *handler.queryParentStar({StmtType::None, 2}, {StmtType::None, 7});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      *handler.queryParentStar({StmtType::None, 1}, {StmtType::Print, 0});
  REQUIRE(result3.secondArgVals == StmtValueSet({3, 4, 7}));

  auto result4 =
      *handler.queryParentStar({StmtType::While, 0}, {StmtType::None, 6});
  REQUIRE(result4.firstArgVals == StmtValueSet({1}));
}

TEST_CASE("ParentStar 2 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  // 1(->2(->3|4),5,6-(>7))
  writer.addParent(1, 2);
  writer.addParent(1, 5);
  writer.addParent(1, 6);
  writer.addParent(2, 3);
  writer.addParent(2, 4);
  writer.addParent(6, 7);
  writer.addFollows(2, 5);
  writer.addFollows(2, 6);
  writer.addFollows(5, 6);
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::Read);
  writer.addStatement(6, StmtType::While);
  writer.addStatement(7, StmtType::Print);
  writer.runPostProcessor();

  auto result1 =
      *handler.queryParentStar({StmtType::While, 0}, {StmtType::Print, 0});
  REQUIRE(result1.pairVals ==
          pair_set<int, int>({{1, 3}, {1, 4}, {1, 7}, {6, 7}}));

  auto result2 =
      *handler.queryParentStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2},
                                                  {1, 3},
                                                  {1, 4},
                                                  {1, 5},
                                                  {1, 6},
                                                  {1, 7},
                                                  {2, 3},
                                                  {2, 4},
                                                  {6, 7}}));
  // wildcard
  auto result3 =
      *handler.queryParentStar({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result3.secondArgVals == unordered_set<int>({2, 3, 4, 5, 6, 7}));

  auto result4 =
      *handler.queryParentStar({StmtType::Wildcard, 0}, {StmtType::None, 4});
  REQUIRE(result4.isEmpty == false);

  auto result5 =
      *handler.queryParentStar({StmtType::Wildcard, 0}, {StmtType::If, 0});
  REQUIRE(result5.secondArgVals == unordered_set<int>({2}));
}
