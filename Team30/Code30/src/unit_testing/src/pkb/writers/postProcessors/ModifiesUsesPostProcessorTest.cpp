#include <memory>
#include "common/Types.h"

#include "catch.hpp"
#include "pkb/writers/PkbWriter.h"
#include "pkb/writers/postProcessors/ModifiesUsesPostProcessor.h"

using std::make_unique;

// procedure main {
// 1  while(y < 1) {
// 2    read x;
// 3    call foo;
//   }
// }
// procedure foo {
// 4  z = 1;
// 5  call goo;
// }
// procedure goo {
// 6  w = x;
// 7  print z;
// }
// procedure hoo {
// 8  call goo;
// }

struct MUPostProcessorTestInit {
  unique_ptr<PKB> pkb;
  PkbWriter writer;
  MUPostProcessorTestInit()
      : pkb(make_unique<PKB>()), writer(PkbWriter(pkb.get())) {
    writer.addParent(1, 2);
    writer.addParent(1, 3);
    writer.addModifies(1, "x", "main");
    writer.addModifies(2, "x", "main");
    writer.addUses(1, "y", "main");
    writer.addCalls(3, "main", "foo");
    writer.addStatement(3, StmtType::Call);
    writer.addProcedure("main", 1, 3);
    writer.addModifies(4, "z", "foo");
    writer.addCalls(5, "foo", "goo");
    writer.addStatement(5, StmtType::Call);
    writer.addProcedure("foo", 4, 5);
    writer.addModifies(6, "w", "goo");
    writer.addUses(6, "x", "goo");
    writer.addUses(7, "z", "goo");
    writer.addProcedure("goo", 6, 7);
    writer.addCalls(8, "hoo", "goo");
    writer.addStatement(8, StmtType::Call);
    writer.addProcedure("hoo", 8, 8);
  };
};

TEST_CASE("ModifiesUsesPostProcessorTest assert initial modifiesPStorage") {
  auto test = MUPostProcessorTestInit();

  auto m1 = test.pkb->modifiesPTable->get("main");
  REQUIRE(m1 == EntityValueSet({"x"}));
  auto m2 = test.pkb->modifiesPTable->get("foo");
  REQUIRE(m2 == EntityValueSet({"z"}));
  auto m3 = test.pkb->modifiesPTable->get("goo");
  REQUIRE(m3 == EntityValueSet({"w"}));
  auto m4 = test.pkb->modifiesPTable->get("hoo");
  REQUIRE(m4 == EntityValueSet({}));

  auto rm1 = test.pkb->modifiesPRevTable->get("x");
  REQUIRE(rm1 == EntityValueSet({"main"}));
  auto rm2 = test.pkb->modifiesPRevTable->get("z");
  REQUIRE(rm2 == EntityValueSet({"foo"}));
  auto rm3 = test.pkb->modifiesPRevTable->get("w");
  REQUIRE(rm3 == EntityValueSet({"goo"}));

  StmtValue calls[] = {3, 5, 8};
  for (StmtValue i : calls) {
    REQUIRE(test.pkb->modifiesTable->get(i).size() == 0);
  }
  REQUIRE(test.pkb->modifiesRevTable->get("x") == StmtValueSet({1, 2}));
  REQUIRE(test.pkb->modifiesRevTable->get("z") == StmtValueSet({4}));
  REQUIRE(test.pkb->modifiesRevTable->get("w") == StmtValueSet({6}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial modifiesStorage") {
  auto test = MUPostProcessorTestInit();

  StmtValue calls[] = {3, 5, 8};
  for (StmtValue i : calls) {
    REQUIRE(test.pkb->modifiesTable->get(i).size() == 0);
  }
  REQUIRE(test.pkb->modifiesRevTable->get("x") == StmtValueSet({1, 2}));
  REQUIRE(test.pkb->modifiesRevTable->get("z") == StmtValueSet({4}));
  REQUIRE(test.pkb->modifiesRevTable->get("w") == StmtValueSet({6}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial usesPStorage") {
  auto test = MUPostProcessorTestInit();

  auto u1 = test.pkb->usesPTable->get("main");
  REQUIRE(u1 == EntityValueSet({"y"}));
  auto u2 = test.pkb->usesPTable->get("foo");
  REQUIRE(u2 == EntityValueSet({}));
  auto u3 = test.pkb->usesPTable->get("goo");
  REQUIRE(u3 == EntityValueSet({"x", "z"}));
  auto u4 = test.pkb->usesPTable->get("hoo");
  REQUIRE(u4 == EntityValueSet({}));

  auto ru1 = test.pkb->usesPRevTable->get("x");
  REQUIRE(ru1 == EntityValueSet({"goo"}));
  auto ru2 = test.pkb->usesPRevTable->get("y");
  REQUIRE(ru2 == EntityValueSet({"main"}));
  auto ru3 = test.pkb->usesPRevTable->get("z");
  REQUIRE(ru3 == EntityValueSet({"goo"}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial usesStorage") {
  auto test = MUPostProcessorTestInit();

  StmtValue calls[] = {3, 5, 8};
  for (StmtValue i : calls) {
    REQUIRE(test.pkb->usesTable->get(i).size() == 0);
  }
  REQUIRE(test.pkb->usesRevTable->get("y") == StmtValueSet({1}));
  REQUIRE(test.pkb->usesRevTable->get("x") == StmtValueSet({6}));
  REQUIRE(test.pkb->usesRevTable->get("z") == StmtValueSet({7}));
}

TEST_CASE(
    "ModifiesUsesPostProcessorTest check post-processed modifiesPStorage") {
  auto test = MUPostProcessorTestInit();
  test.writer.runPostProcessor();

  auto m1 = test.pkb->modifiesPTable->get("main");
  REQUIRE(m1 == EntityValueSet({"x", "z", "w"}));
  auto m2 = test.pkb->modifiesPTable->get("foo");
  REQUIRE(m2 == EntityValueSet({"z", "w"}));
  auto m3 = test.pkb->modifiesPTable->get("goo");
  REQUIRE(m3 == EntityValueSet({"w"}));
  auto m4 = test.pkb->modifiesPTable->get("hoo");
  REQUIRE(m4 == EntityValueSet({"w"}));

  auto rm1 = test.pkb->modifiesPRevTable->get("x");
  REQUIRE(rm1 == EntityValueSet({"main"}));
  auto rm2 = test.pkb->modifiesPRevTable->get("z");
  REQUIRE(rm2 == EntityValueSet({"foo", "main"}));
  auto rm3 = test.pkb->modifiesPRevTable->get("w");
  REQUIRE(rm3 == EntityValueSet({"goo", "hoo", "main", "foo"}));
}

TEST_CASE(
    "ModifiesUsesPostProcessorTest check post-processed modifiesStorage") {
  auto test = MUPostProcessorTestInit();
  test.writer.runPostProcessor();

  auto c1 = test.pkb->modifiesTable->get(3);
  REQUIRE(c1 == EntityValueSet({"w", "z"}));
  auto c2 = test.pkb->modifiesTable->get(5);
  REQUIRE(c2 == EntityValueSet({"w"}));
  auto c3 = test.pkb->modifiesTable->get(8);
  REQUIRE(c3 == EntityValueSet({"w"}));
  // while container
  auto c4 = test.pkb->modifiesTable->get(1);
  REQUIRE(c4 == EntityValueSet({"x", "w", "z"}));

  auto cr1 = test.pkb->modifiesRevTable->get("x");
  REQUIRE(cr1 == StmtValueSet({1, 2}));
  auto cr2 = test.pkb->modifiesRevTable->get("z");
  REQUIRE(cr2 == StmtValueSet({1, 4, 3}));
  auto cr3 = test.pkb->modifiesRevTable->get("w");
  REQUIRE(cr3 == StmtValueSet({1, 6, 3, 5, 8}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed usesPStorage") {
  auto test = MUPostProcessorTestInit();
  test.writer.runPostProcessor();

  auto u1 = test.pkb->usesPTable->get("main");
  REQUIRE(u1 == EntityValueSet({"y", "x", "z"}));
  auto u2 = test.pkb->usesPTable->get("foo");
  REQUIRE(u2 == EntityValueSet({"x", "z"}));
  auto u3 = test.pkb->usesPTable->get("goo");
  REQUIRE(u3 == EntityValueSet({"x", "z"}));
  auto u4 = test.pkb->usesPTable->get("hoo");
  REQUIRE(u4 == EntityValueSet({"x", "z"}));

  auto ru1 = test.pkb->usesPRevTable->get("x");
  REQUIRE(ru1 == EntityValueSet({"goo", "main", "foo", "hoo"}));
  auto ru2 = test.pkb->usesPRevTable->get("y");
  REQUIRE(ru2 == EntityValueSet({"main"}));
  auto ru3 = test.pkb->usesPRevTable->get("z");
  REQUIRE(ru3 == EntityValueSet({"goo", "main", "foo", "hoo"}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed usesStorage") {
  auto test = MUPostProcessorTestInit();
  test.writer.runPostProcessor();

  auto c1 = test.pkb->usesTable->get(3);
  REQUIRE(c1 == EntityValueSet({"x", "z"}));
  auto c2 = test.pkb->usesTable->get(5);
  REQUIRE(c2 == EntityValueSet({"x", "z"}));
  auto c3 = test.pkb->usesTable->get(8);
  REQUIRE(c3 == EntityValueSet({"x", "z"}));
  // while container
  auto c4 = test.pkb->usesTable->get(1);
  REQUIRE(c4 == EntityValueSet({"y", "x", "z"}));

  auto cr1 = test.pkb->usesRevTable->get("y");
  REQUIRE(cr1 == StmtValueSet({1}));
  auto cr2 = test.pkb->usesRevTable->get("x");
  REQUIRE(cr2 == StmtValueSet({1, 6, 3, 5, 8}));
  auto cr3 = test.pkb->usesRevTable->get("z");
  REQUIRE(cr3 == StmtValueSet({1, 7, 3, 5, 8}));
}

// procedure main {
// 1  while(0 < 1) {
// 2    if (y == 2) {
// 3	      read x;
// 4       call foo;
//      }
//   }
// }
// procedure foo {
// 5  z = 1;
// 6  print z;
// }

struct MUPostProcessorTest2 {
  unique_ptr<PKB> pkb;
  PkbWriter writer;
  MUPostProcessorTest2()
      : pkb(make_unique<PKB>()), writer(PkbWriter(pkb.get())) {
    writer.addParent(1, 2);
    writer.addParent(2, 3);
    writer.addParent(2, 4);
    writer.addModifies(1, "x", "main");
    writer.addModifies(2, "x", "main");
    writer.addModifies(3, "x", "main");
    writer.addUses(2, "y", "main");
    writer.addCalls(4, "main", "foo");
    writer.addStatement(4, StmtType::Call);
    writer.addProcedure("main", 1, 4);
    writer.addModifies(5, "z", "foo");
    writer.addUses(6, "z", "foo");
    writer.addProcedure("foo", 5, 6);
  };
};

TEST_CASE("ModifiesUsesPostProcessorTest check modifies for ontainer stmts") {
  auto test = MUPostProcessorTest2();
  test.writer.runPostProcessor();

  auto c1 = test.pkb->modifiesTable->get(1);
  REQUIRE(c1 == EntityValueSet({"x", "z"}));
  auto c2 = test.pkb->modifiesTable->get(2);
  REQUIRE(c2 == EntityValueSet({"x", "z"}));
  auto c3 = test.pkb->modifiesTable->get(4);
  REQUIRE(c3 == EntityValueSet({"z"}));

  auto cr1 = test.pkb->modifiesRevTable->get("x");
  REQUIRE(cr1 == StmtValueSet({1, 2, 3}));
  auto cr2 = test.pkb->modifiesRevTable->get("z");
  REQUIRE(cr2 == StmtValueSet({1, 2, 4, 5}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check uses for ontainer stmts") {
  auto test = MUPostProcessorTest2();
  test.writer.runPostProcessor();

  auto c1 = test.pkb->usesTable->get(1);
  REQUIRE(c1 == EntityValueSet({"z"}));
  auto c2 = test.pkb->usesTable->get(2);
  REQUIRE(c2 == EntityValueSet({"y", "z"}));
  auto c3 = test.pkb->usesTable->get(4);
  REQUIRE(c3 == EntityValueSet({"z"}));

  auto cr1 = test.pkb->usesRevTable->get("y");
  REQUIRE(cr1 == StmtValueSet({2}));
  auto cr2 = test.pkb->usesRevTable->get("z");
  REQUIRE(cr2 == StmtValueSet({1, 2, 4, 6}));
}
