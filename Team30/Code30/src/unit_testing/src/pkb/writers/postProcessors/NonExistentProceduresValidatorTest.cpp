#include <memory>

#include "catch.hpp"
#include "pkb/writers/PkbWriter.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/postProcessors/NonExistentProceduresValidator.h"
#include "pkb/errors/PKBError.h"

TEST_CASE("NonExistentProceduresValidator post-declaration no error ") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);

  auto validator = NonExistentProceduresValidator(pkb.get());
  REQUIRE_NOTHROW(validator.validate());
}

TEST_CASE("NonExistentProceduresValidator pre-declaration no error ") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(2, "main", "foo");
  writer.addProcedure("foo", 1, 1);
  writer.addProcedure("main", 2, 2);

  auto validator = NonExistentProceduresValidator(pkb.get());
  REQUIRE_NOTHROW(validator.validate());
}

TEST_CASE("NonExistentProceduresValidator throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addProcedure("main", 1, 1);

  auto validator = NonExistentProceduresValidator(pkb.get());
  REQUIRE_THROWS_AS(validator.validate(), PKBError);
}
