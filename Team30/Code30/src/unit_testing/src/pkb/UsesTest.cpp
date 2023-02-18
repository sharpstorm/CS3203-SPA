#include <unordered_set>
#include <memory>
#include <string>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set;
using std::make_unique;
using std::string;

TEST_CASE("Uses (StmtRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addUses(3, "x");
  writer.addUses(4, "x");
  writer.addUses(4, "y");
  writer.addUses(1, "x");
  writer.addUses(1, "y");
  writer.addUses(2, "x");
  writer.addUses(2, "y");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Assign);
  writer.addSymbol("x", EntityType::Variable);
  writer.addSymbol("y", EntityType::Variable);
  writer.addSymbol("z", EntityType::Variable);

  auto result1 =
      handler.queryUses({StmtType::None, 1}, {EntityType::None, "y"});
  REQUIRE(result1.pairVals == pair_set<int, string>({{1, "y"}}));

  auto result2 =
      handler.queryUses({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result2.pairVals == pair_set<int, string>({{1, "y"}, {1, "x"}}));

  auto result3 =
      handler.queryUses({StmtType::Assign, 0}, {EntityType::None, "x"});
  REQUIRE(result3.pairVals == pair_set<int, string>({{4, "x"}}));

  auto result4 =
      handler.queryUses({StmtType::None, 0}, {EntityType::Variable, ""});
  REQUIRE(result4.pairVals
              == pair_set<int, string>({{1, "x"}, {1, "y"}, {2, "x"}, {2, "y"},
                                        {3, "x"}, {4, "x"}, {4, "y"},}));
}

TEST_CASE("Uses (EntityRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addUses(2, "x");
  writer.addUses(3, "z");
  writer.addUses(4, "z");
  writer.addUses(1, "x");
  writer.addUses(1, "y");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Assign);
  writer.addStatement(4, StmtType::Assign);
  writer.addStatement(5, StmtType::Assign);
  writer.addSymbol("x", EntityType::Variable);
  writer.addSymbol("y", EntityType::Variable);
  writer.addSymbol("z", EntityType::Variable);
  writer.addSymbol("w", EntityType::Variable);
  writer.addProcedure("main", 1, 3);
  writer.addProcedure("foo", 4, 4);

  auto result1 =
      handler.queryUses({EntityType::Procedure, "foo"},
                        {EntityType::None, "z"});
  REQUIRE(result1.pairVals == pair_set<string, string>({{"foo", "z"}}));

  auto result2 =
      handler.queryUses({EntityType::Procedure, "main"},
                        {EntityType::Variable, ""});
  REQUIRE(result2.pairVals
              == pair_set<string, string>({{"main", "x"}, {"main", "y"},
                                           {"main", "z"}}));

  auto result3 =
      handler.queryUses({EntityType::Procedure, ""},
                        {EntityType::None, "z"});
  REQUIRE(result3.pairVals
              == pair_set<string, string>({{"main", "z"}, {"foo", "z"}}));

  auto result4 =
      handler.queryUses({EntityType::Procedure, ""},
                        {EntityType::Variable, ""});
  REQUIRE(result4.pairVals
              == pair_set<string, string>({{"main", "x"}, {"main", "y"},
                                           {"main", "z"}, {"foo", "z"}}));
}

