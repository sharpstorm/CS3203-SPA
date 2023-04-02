#include <memory>

#include "catch.hpp"
#include "pkb/errors/PKBError.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/writers/postProcessors/CallsTPostProcessor.h"
#include "pkb/writers/postProcessors/CyclicProceduresValidator.h"

TEST_CASE("CyclicProceduresValidator no error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addCalls(2, "main", "goo");
  writer.addCalls(3, "foo", "goo");
  writer.addProcedure("main", 1, 2);
  writer.addProcedure("foo", 3, 3);
  writer.addProcedure("goo", 4, 4);
  auto callsTProcessor = CallsTPostProcessor(pkb.get());
  callsTProcessor.process();
  auto validator = CyclicProceduresValidator(pkb.get());
  REQUIRE_NOTHROW(validator.validate());
}

TEST_CASE("CyclicProceduresValidator recursive call throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "main");
  writer.addProcedure("main", 1, 1);
  auto callsTProcessor = CallsTPostProcessor(pkb.get());
  callsTProcessor.process();

  auto validator = CyclicProceduresValidator(pkb.get());
  REQUIRE_THROWS_AS(validator.validate(), PKBError);
}

TEST_CASE("CyclicProceduresValidator cyclic call 1 throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addCalls(2, "foo", "main");
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);
  auto callsTProcessor = CallsTPostProcessor(pkb.get());
  callsTProcessor.process();

  auto validator = CyclicProceduresValidator(pkb.get());
  REQUIRE_THROWS_AS(validator.validate(), PKBError);
}

TEST_CASE("CyclicProceduresValidator cyclic call 2 throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addCalls(2, "foo", "goo");
  writer.addCalls(3, "goo", "main");
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);
  writer.addProcedure("goo", 3, 3);
  auto callsTProcessor = CallsTPostProcessor(pkb.get());
  callsTProcessor.process();

  auto validator = CyclicProceduresValidator(pkb.get());
  REQUIRE_THROWS_AS(validator.validate(), PKBError);
}
