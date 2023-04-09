#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

TEST_CASE("WhilePattern unknown while") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addWhilePattern(1, "a");
  writer.addWhilePattern(1, "b");
  writer.addWhilePattern(2, "a");
  writer.addWhilePattern(3, "c");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::While);
  writer.addStatement(3, StmtType::While);
  writer.addStatement(4, StmtType::While);
  writer.addStatement(5, StmtType::If);

  auto res1 =
      *handler.queryWhilePattern({StmtType::While, 0}, {EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == StmtValueSet({1, 2}));

  auto res2 = *handler.queryWhilePattern({StmtType::While, 0},
                                         {EntityType::Variable, ""});
  REQUIRE(res2.pairVals == pair_set<StmtValue, EntityValue>(
                               {{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  auto res3 = *handler.queryWhilePattern({StmtType::While, 0},
                                         {EntityType::Wildcard, ""});
  REQUIRE(res3.firstArgVals == StmtValueSet({1, 2, 3}));
}

TEST_CASE("WhilePattern known while") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addWhilePattern(1, "a");
  writer.addWhilePattern(1, "b");
  writer.addWhilePattern(2, "a");
  writer.addWhilePattern(3, "c");

  auto res1 =
      *handler.queryWhilePattern({StmtType::None, 1}, {EntityType::None, "a"});
  REQUIRE(res1.isEmpty == false);

  auto res2 = *handler.queryWhilePattern({StmtType::None, 1},
                                         {EntityType::Variable, ""});
  REQUIRE(res2.secondArgVals == EntityValueSet({"a", "b"}));

  auto res3 = *handler.queryWhilePattern({StmtType::None, 2},
                                         {EntityType::Wildcard, ""});
  REQUIRE(res3.isEmpty == false);
}
