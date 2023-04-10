#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

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

  auto res1 =
      *handler.queryIfPattern({StmtType::If, 0}, {EntityType::None, "a"});
  REQUIRE(res1.getLeftVals() == StmtValueSet({1, 2}));

  auto res2 =
      *handler.queryIfPattern({StmtType::If, 0}, {EntityType::Variable, ""});
  REQUIRE(res2.getPairVals() == pair_set<StmtValue, EntityValue>(
                                    {{1, "a"}, {1, "b"}, {2, "a"}, {3, "c"}}));

  auto res3 =
      *handler.queryIfPattern({StmtType::If, 0}, {EntityType::Wildcard, ""});
  REQUIRE(res3.getLeftVals() == StmtValueSet({1, 2, 3}));
}

TEST_CASE("IfPattern known if") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addIfPattern(1, "a");
  writer.addIfPattern(1, "b");
  writer.addIfPattern(2, "a");
  writer.addIfPattern(3, "c");

  auto res1 =
      *handler.queryIfPattern({StmtType::None, 1}, {EntityType::None, "a"});
  REQUIRE(res1.empty() == false);

  auto res2 =
      *handler.queryIfPattern({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(res2.getRightVals() == EntityValueSet({"a", "b"}));

  auto res3 =
      *handler.queryIfPattern({StmtType::None, 2}, {EntityType::Wildcard, ""});
  REQUIRE(res3.empty() == false);
}
