
#include "catch.hpp"
#include "qps/cfg/cfg_querier/writers/CFGResultWriterFactory.h"
#include "../CFGTestModifiesUsesProvider.h"
#include "../TestCFGProvider.h"

TEST_CASE("CFG Writer Permutations, Bool") {
  CFG cfg = TestCFGProvider::getLinearCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makeBoolWriter<CFGTestModifiesUsesProvider::typePredicate>(1, 2);

  REQUIRE(boolWriter->writeBool(1));
  REQUIRE_FALSE(boolWriter->writeBool(2));
  REQUIRE(boolWriter->writeBool(3));
}

TEST_CASE("CFG Writer Permutations - Const, Syn") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makeRightWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                          StmtType::Assign);
  REQUIRE(boolWriter->writeRight(1));
  REQUIRE(boolWriter->writeRight(2));
  REQUIRE(boolWriter->writeRight(3));
  REQUIRE(boolWriter->writeRight(4));

  REQUIRE(result.getRightVals() == QueryResultSet<StmtValue>{1, 2, 4});
}

TEST_CASE("CFG Writer Permutations - Const, Wildcard") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makeRightWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                          StmtType::Wildcard);
  REQUIRE_FALSE(boolWriter->writeRight(2));
  REQUIRE(!result.empty());
  REQUIRE_FALSE(boolWriter->writeRight(3));
}

TEST_CASE("CFG Writer Permutations - Syn, Const") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makeLeftWriter<CFGTestModifiesUsesProvider::typePredicate>(StmtType::Assign,
                                                                         4);
  REQUIRE(boolWriter->writeLeft(3));
  REQUIRE(boolWriter->writeLeft(2));
  REQUIRE(boolWriter->writeLeft(1));
  REQUIRE(!result.empty());
  REQUIRE(result.getLeftVals() == QueryResultSet<StmtValue>{1,2});
}

TEST_CASE("CFG Writer Permutations - Syn, Syn") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makePairWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                         StmtType::Assign,
                                                                         StmtType::Assign);
  REQUIRE(boolWriter->writeRight(2));
  REQUIRE(boolWriter->writeRight(3));
  REQUIRE(boolWriter->writeRight(4));
  REQUIRE(!result.empty());
  QueryResultPairSet<StmtValue, StmtValue> copy = result.getPairVals();
  REQUIRE(copy == QueryResultPairSet<StmtValue, StmtValue>{
      {1, 2},
      {1, 4},
  });
}

TEST_CASE("CFG Writer Permutations - Syn, Wildcard") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makePairWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                         StmtType::Assign,
                                                                         StmtType::Wildcard);
  REQUIRE_FALSE(boolWriter->writeRight(3));
  REQUIRE_FALSE(boolWriter->writeRight(2));
  REQUIRE(!result.empty());
  REQUIRE(result.getLeftVals() == QueryResultSet<StmtValue>{1});
}

TEST_CASE("CFG Writer Permutations - Wildcard, Const") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makeLeftWriter<CFGTestModifiesUsesProvider::typePredicate>(StmtType::Wildcard,
                                                                         4);
  REQUIRE_FALSE(boolWriter->writeLeft(3));
  REQUIRE(!result.empty());
  REQUIRE_FALSE(boolWriter->writeLeft(2));
}

TEST_CASE("CFG Writer Permutations - Wildcard, Syn") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makePairWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                         StmtType::Wildcard,
                                                                         StmtType::Assign);
  REQUIRE(boolWriter->writeRight(1));
  REQUIRE(boolWriter->writeRight(2));
  REQUIRE(boolWriter->writeRight(3));
  REQUIRE(boolWriter->writeRight(4));

  REQUIRE(result.getRightVals() == QueryResultSet<StmtValue>{1,2,4});
}

TEST_CASE("CFG Writer Permutations - Wildcard, Wildcard") {
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);

  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  {{3, StmtType::If}},
                                  &cache);
  StmtTransitiveResult result;
  auto factory = makeCFGResultWriterFactory(&cfg, &pkbProvider, &result);
  auto boolWriter =
      factory.makePairWriter<CFGTestModifiesUsesProvider::typePredicate>(1,
                                                                         StmtType::Wildcard,
                                                                         StmtType::Wildcard);
  REQUIRE_FALSE(boolWriter->writeRight(1));
  REQUIRE(!result.empty());
  REQUIRE_FALSE(boolWriter->writeRight(3));
}
