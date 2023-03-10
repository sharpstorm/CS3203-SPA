#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/writers/postProcessors/ModifiesUsesPostProcessor.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

//procedure main {
//1  while(y < 1) {
//2    read x;
//3    call foo;
//  }
//}
//procedure foo {
//4  z = 1;
//5  call goo;
//}
//procedure goo {
//6  w = x;
//7  print z;
//}
//procedure hoo {
//8  call goo;
//}

struct MUPostProcessorTestInit {
  unique_ptr<PKB> pkb;
  PkbWriter writer;
  ModifiesUsesPostProcessor processor;
  MUPostProcessorTestInit()
      : pkb(make_unique<PKB>()),
        writer(PkbWriter(pkb.get())),
        processor(ModifiesUsesPostProcessor(pkb.get())) {
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

  auto m1 = test.pkb->modifiesPStorage->getByFirstArg("main");
  REQUIRE(m1 == unordered_set<string>({"x"}));
  auto m2 = test.pkb->modifiesPStorage->getByFirstArg("foo");
  REQUIRE(m2 == unordered_set<string>({"z"}));
  auto m3 = test.pkb->modifiesPStorage->getByFirstArg("goo");
  REQUIRE(m3 == unordered_set<string>({"w"}));
  auto m4 = test.pkb->modifiesPStorage->getByFirstArg("hoo");
  REQUIRE(m4 == unordered_set<string>({}));

  auto rm1 = test.pkb->modifiesPStorage->getBySecondArg("x");
  REQUIRE(rm1 == unordered_set<string>({"main"}));
  auto rm2 = test.pkb->modifiesPStorage->getBySecondArg("z");
  REQUIRE(rm2 == unordered_set<string>({"foo"}));
  auto rm3 = test.pkb->modifiesPStorage->getBySecondArg("w");
  REQUIRE(rm3 == unordered_set<string>({"goo"}));

  int calls[] = {3, 5, 8};
  for (int i : calls) {
    REQUIRE(test.pkb->modifiesStorage->getByFirstArg(i).size() == 0);
  }
  REQUIRE(
      test.pkb->modifiesStorage->getBySecondArg("x") == unordered_set({1, 2}));
  REQUIRE(test.pkb->modifiesStorage->getBySecondArg("z") == unordered_set({4}));
  REQUIRE(test.pkb->modifiesStorage->getBySecondArg("w") == unordered_set({6}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial modifiesStorage") {
  auto test = MUPostProcessorTestInit();

  int calls[] = {3, 5, 8};
  for (int i : calls) {
    REQUIRE(test.pkb->modifiesStorage->getByFirstArg(i).size() == 0);
  }
  REQUIRE(
      test.pkb->modifiesStorage->getBySecondArg("x") == unordered_set({1, 2}));
  REQUIRE(test.pkb->modifiesStorage->getBySecondArg("z") == unordered_set({4}));
  REQUIRE(test.pkb->modifiesStorage->getBySecondArg("w") == unordered_set({6}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial usesPStorage") {
  auto test = MUPostProcessorTestInit();

  auto u1 = test.pkb->usesPStorage->getByFirstArg("main");
  REQUIRE(u1 == unordered_set<string>({"y"}));
  auto u2 = test.pkb->usesPStorage->getByFirstArg("foo");
  REQUIRE(u2 == unordered_set<string>({}));
  auto u3 = test.pkb->usesPStorage->getByFirstArg("goo");
  REQUIRE(u3 == unordered_set<string>({"x", "z"}));
  auto u4 = test.pkb->usesPStorage->getByFirstArg("hoo");
  REQUIRE(u4 == unordered_set<string>({}));

  auto ru1 = test.pkb->usesPStorage->getBySecondArg("x");
  REQUIRE(ru1 == unordered_set<string>({"goo"}));
  auto ru2 = test.pkb->usesPStorage->getBySecondArg("y");
  REQUIRE(ru2 == unordered_set<string>({"main"}));
  auto ru3 = test.pkb->usesPStorage->getBySecondArg("z");
  REQUIRE(ru3 == unordered_set<string>({"goo"}));
}

TEST_CASE("ModifiesUsesPostProcessorTest assert initial usesStorage") {
  auto test = MUPostProcessorTestInit();

  int calls[] = {3, 5, 8};
  for (int i : calls) {
    REQUIRE(test.pkb->usesStorage->getByFirstArg(i).size() == 0);
  }
  REQUIRE(
      test.pkb->usesStorage->getBySecondArg("y") == unordered_set({1}));
  REQUIRE(test.pkb->usesStorage->getBySecondArg("x") == unordered_set({6}));
  REQUIRE(test.pkb->usesStorage->getBySecondArg("z") == unordered_set({7}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed modifiesPStorage") {
  auto test = MUPostProcessorTestInit();
  test.processor.process();

  auto m1 = test.pkb->modifiesPStorage->getByFirstArg("main");
  REQUIRE(m1 == unordered_set<string>({"x", "z", "w"}));
  auto m2 = test.pkb->modifiesPStorage->getByFirstArg("foo");
  REQUIRE(m2 == unordered_set<string>({"z", "w"}));
  auto m3 = test.pkb->modifiesPStorage->getByFirstArg("goo");
  REQUIRE(m3 == unordered_set<string>({"w"}));
  auto m4 = test.pkb->modifiesPStorage->getByFirstArg("hoo");
  REQUIRE(m4 == unordered_set<string>({"w"}));

  auto rm1 = test.pkb->modifiesPStorage->getBySecondArg("x");
  REQUIRE(rm1 == unordered_set<string>({"main"}));
  auto rm2 = test.pkb->modifiesPStorage->getBySecondArg("z");
  REQUIRE(rm2 == unordered_set<string>({"foo", "main"}));
  auto rm3 = test.pkb->modifiesPStorage->getBySecondArg("w");
  REQUIRE(rm3 == unordered_set<string>({"goo", "hoo", "main", "foo"}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed modifiesStorage") {
  auto test = MUPostProcessorTestInit();
  test.processor.process();

  auto c1 = test.pkb->modifiesStorage->getByFirstArg(3);
  REQUIRE(c1 == unordered_set<string>({"w", "z"}));
  auto c2 = test.pkb->modifiesStorage->getByFirstArg(5);
  REQUIRE(c2 == unordered_set<string>({"w"}));
  auto c3 = test.pkb->modifiesStorage->getByFirstArg(8);
  REQUIRE(c3 == unordered_set<string>({"w"}));

  auto cr1 = test.pkb->modifiesStorage->getBySecondArg("x");
  REQUIRE(cr1 == unordered_set({1, 2}));
  auto cr2 = test.pkb->modifiesStorage->getBySecondArg("z");
  REQUIRE(cr2 == unordered_set({4, 3}));
  auto cr3 = test.pkb->modifiesStorage->getBySecondArg("w");
  REQUIRE(cr3 == unordered_set({6, 3, 5, 8}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed usesPStorage") {
  auto test = MUPostProcessorTestInit();
  test.processor.process();

  auto u1 = test.pkb->usesPStorage->getByFirstArg("main");
  REQUIRE(u1 == unordered_set<string>({"y", "x", "z"}));
  auto u2 = test.pkb->usesPStorage->getByFirstArg("foo");
  REQUIRE(u2 == unordered_set<string>({"x", "z"}));
  auto u3 = test.pkb->usesPStorage->getByFirstArg("goo");
  REQUIRE(u3 == unordered_set<string>({"x", "z"}));
  auto u4 = test.pkb->usesPStorage->getByFirstArg("hoo");
  REQUIRE(u4 == unordered_set<string>({"x", "z"}));

  auto ru1 = test.pkb->usesPStorage->getBySecondArg("x");
  REQUIRE(ru1 == unordered_set<string>({"goo", "main", "foo", "hoo"}));
  auto ru2 = test.pkb->usesPStorage->getBySecondArg("y");
  REQUIRE(ru2 == unordered_set<string>({"main"}));
  auto ru3 = test.pkb->usesPStorage->getBySecondArg("z");
  REQUIRE(ru3 == unordered_set<string>({"goo", "main", "foo", "hoo"}));
}

TEST_CASE("ModifiesUsesPostProcessorTest check post-processed usesStorage") {
  auto test = MUPostProcessorTestInit();
  test.processor.process();

  auto c1 = test.pkb->usesStorage->getByFirstArg(3);
  REQUIRE(c1 == unordered_set<string>({"x", "z"}));
  auto c2 = test.pkb->usesStorage->getByFirstArg(5);
  REQUIRE(c2 == unordered_set<string>({"x", "z"}));
  auto c3 = test.pkb->usesStorage->getByFirstArg(8);
  REQUIRE(c3 == unordered_set<string>({"x", "z"}));

  auto cr1 = test.pkb->usesStorage->getBySecondArg("y");
  REQUIRE(cr1 == unordered_set({1}));
  auto cr2 = test.pkb->usesStorage->getBySecondArg("x");
  REQUIRE(cr2 == unordered_set({6, 3, 5, 8}));
  auto cr3 = test.pkb->usesStorage->getBySecondArg("z");
  REQUIRE(cr3 == unordered_set({7, 3, 5, 8}));
}
