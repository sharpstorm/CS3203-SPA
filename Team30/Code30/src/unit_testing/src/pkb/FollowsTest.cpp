#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique, std::unique_ptr;

struct followsTest {
  unique_ptr<PKB> pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler handler = PkbQueryHandler(pkb.get());

  followsTest() {
    // 1->2->3->4
    writer.addFollows(1, 2);
    writer.addFollows(1, 3);
    writer.addFollows(1, 4);
    writer.addFollows(2, 3);
    writer.addFollows(2, 4);
    writer.addFollows(3, 4);
    writer.addStatement(1, StmtType::Assign);
    writer.addStatement(2, StmtType::Read);
    writer.addStatement(3, StmtType::Read);
    writer.addStatement(4, StmtType::Print);
  }
};

TEST_CASE("Follows") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollows({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      *test.handler.queryFollows({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      *test.handler.queryFollows({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result3.secondArgVals == StmtValueSet({2}));
  auto result4 =
      *test.handler.queryFollows({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result4.firstArgVals == StmtValueSet({3}));
}

TEST_CASE("Follows 2 unknowns") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollows({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<StmtValue, StmtValue>({{1, 2}, {2, 3}}));

  auto result2 =
      *test.handler.queryFollows({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals ==
          pair_set<StmtValue, StmtValue>({{1, 2}, {2, 3}, {3, 4}}));

  auto result5 =
      *test.handler.queryFollows({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result5.pairVals == pair_set<StmtValue, StmtValue>({{3, 4}}));

  auto result6 =
      *test.handler.queryFollows({StmtType::None, 0}, {StmtType::None, 4});
  REQUIRE(result6.firstArgVals == StmtValueSet({3}));
}

TEST_CASE("Follows wildcard rightArg") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollows({StmtType::Wildcard, 0}, {StmtType::None, 3});
  REQUIRE(result1.isEmpty == false);
  auto result2 =
      *test.handler.queryFollows({StmtType::Wildcard, 0}, {StmtType::None, 1});
  REQUIRE(result2.isEmpty == true);
  auto result3 =
      *test.handler.queryFollows({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result3.secondArgVals == StmtValueSet({2, 3, 4}));
  auto result4 =
      *test.handler.queryFollows({StmtType::Wildcard, 0}, {StmtType::Read, 0});
  REQUIRE(result4.secondArgVals == StmtValueSet({2, 3}));
}

TEST_CASE("Follows wildcard leftArg") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollows({StmtType::None, 2}, {StmtType::Wildcard, 0});
  REQUIRE(result1.isEmpty == false);
  auto result2 =
      *test.handler.queryFollows({StmtType::None, 4}, {StmtType::Wildcard, 0});
  REQUIRE(result2.isEmpty == true);
  auto result3 =
      *test.handler.queryFollows({StmtType::None, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result3.firstArgVals == StmtValueSet({1, 2, 3}));
  auto result4 =
      *test.handler.queryFollows({StmtType::Read, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result4.firstArgVals == StmtValueSet({2, 3}));
}

TEST_CASE("FollowsStar <= 1 unknown") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 4});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      *test.handler.queryFollowsStar({StmtType::None, 3}, {StmtType::None, 1});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      *test.handler.queryFollowsStar({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result3.secondArgVals == StmtValueSet({2, 3}));

  auto result4 =
      *test.handler.queryFollowsStar({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result4.firstArgVals == StmtValueSet({2, 3}));
}

TEST_CASE("FollowsStar 2 unknowns") {
  auto test = followsTest();

  auto result1 =
      *test.handler.queryFollowsStar({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.pairVals ==
          pair_set<StmtValue, StmtValue>({{1, 2}, {2, 3}, {1, 3}}));

  auto result2 =
      *test.handler.queryFollowsStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals ==
          pair_set<StmtValue, StmtValue>(
              {{1, 2}, {2, 3}, {3, 4}, {1, 3}, {1, 4}, {2, 4}}));

  auto result3 =
      *test.handler.queryFollowsStar({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.pairVals == pair_set<StmtValue, StmtValue>({{2, 4}, {3, 4}}));
}

TEST_CASE("FollowsStar wildcard rightArg") {
  auto test = followsTest();

  auto result1 = *test.handler.queryFollowsStar({StmtType::Wildcard, 0},
                                                {StmtType::None, 3});
  REQUIRE(result1.isEmpty == false);
  auto result2 = *test.handler.queryFollowsStar({StmtType::Wildcard, 0},
                                                {StmtType::None, 1});
  REQUIRE(result2.isEmpty == true);
  auto result3 = *test.handler.queryFollowsStar({StmtType::Wildcard, 0},
                                                {StmtType::None, 0});
  REQUIRE(result3.secondArgVals == StmtValueSet({2, 3, 4}));
  auto result4 = *test.handler.queryFollowsStar({StmtType::Wildcard, 0},
                                                {StmtType::Read, 0});
  REQUIRE(result4.secondArgVals == StmtValueSet({2, 3}));
}

TEST_CASE("FollowsStar wildcard leftArg") {
  auto test = followsTest();

  auto result1 = *test.handler.queryFollowsStar({StmtType::None, 2},
                                                {StmtType::Wildcard, 0});
  REQUIRE(result1.isEmpty == false);
  auto result2 = *test.handler.queryFollowsStar({StmtType::None, 4},
                                                {StmtType::Wildcard, 0});
  REQUIRE(result2.isEmpty == true);
  auto result3 = *test.handler.queryFollowsStar({StmtType::None, 0},
                                                {StmtType::Wildcard, 0});
  REQUIRE(result3.firstArgVals == StmtValueSet({1, 2, 3}));
  auto result4 = *test.handler.queryFollowsStar({StmtType::Read, 0},
                                                {StmtType::Wildcard, 0});
  REQUIRE(result4.firstArgVals == StmtValueSet({2, 3}));
}
