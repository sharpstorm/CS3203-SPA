#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set, std::make_unique;

TEST_CASE("IfPattern unknown if") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addIfPattern(1, "a");
  writer.addIfPattern(1, "b");
  writer.addIfPattern(2, "a");
  writer.addIfPattern(3, "c");
  writer.addStatement(1, StmtType::If);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::If);
  writer.addStatement(4, StmtType::If);
  writer.addStatement(5, StmtType::While);

  auto
      res1 = *handler.queryIfPattern({StmtType::If, 0}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 =
      *handler.queryIfPattern({StmtType::If, 0}, {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  auto res3 = *handler.queryIfPattern({StmtType::If, 0}, {EntityType::None, ""});
  REQUIRE(res3.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res3.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res3.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));
}

TEST_CASE("IfPattern known if") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addIfPattern(1, "a");
  writer.addIfPattern(1, "b");
  writer.addIfPattern(2, "a");
  writer.addIfPattern(3, "c");

  auto
      res1 =
      *handler.queryIfPattern({StmtType::None, 1}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}}));

  auto res2 =
      *handler.queryIfPattern({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}}));

  auto res3 =
      *handler.queryIfPattern({StmtType::None, 2}, {EntityType::None, ""});
  REQUIRE(res3.firstArgVals == unordered_set<int>({2}));
  REQUIRE(res3.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res3.pairVals == pair_set<int, string>({{2, "a"}}));
}
