#include "catch.hpp"

#include "common/Types.h"
#include "qps/common/constraint/OverrideTransformer.h"

TEST_CASE("Transform StmtRef") {
  OverrideTransformer transformer(0);

  // No stmt
  StmtRef inputRef = StmtRef{StmtType::None, NO_STMT};
  StmtRef actual = transformer.transformArg(inputRef);
  StmtRef expected = StmtRef{StmtType::None, INVALID_STMT};
  REQUIRE(expected.getType() == actual.getType());
  REQUIRE(expected.getValue() == actual.getValue());

  transformer = OverrideTransformer(3);
  expected = StmtRef{StmtType::None, 3};
  inputRef = StmtRef{StmtType::None, 3};
  actual = transformer.transformArg(inputRef);
  REQUIRE(expected.getType() == actual.getType());
  REQUIRE(expected.getValue() == actual.getValue());
}

TEST_CASE("Transform EntityRef") {
  OverrideTransformer transformer("");

  // No Ent
  EntityRef inputRef = EntityRef{EntityType::None, NO_ENT};
  EntityRef actual = transformer.transformArg(inputRef);
  REQUIRE(inputRef.getType() == actual.getType());
  REQUIRE(inputRef.getValue() == actual.getValue());

  // Any other ent except for constant
  transformer = OverrideTransformer("x");
  EntityRef expected = EntityRef{EntityType::Variable, "x"};
  inputRef = EntityRef{EntityType::Variable, "g"};
  actual = transformer.transformArg(inputRef);
  REQUIRE(expected.getType() == actual.getType());
  REQUIRE(expected.getValue() == actual.getValue());
}

TEST_CASE("Transform EntityRef - Constant case") {
  OverrideTransformer transformer(0);
  EntityRef inputRef = EntityRef{EntityType::Constant, NO_ENT};
  EntityRef actual = transformer.transformArg(inputRef);
  EntityRef expected = EntityRef{EntityType::Constant, "0"};
  REQUIRE(expected.getType() == actual.getType());
  REQUIRE(expected.getValue() == actual.getValue());

  transformer = OverrideTransformer(5);
  expected = EntityRef{EntityType::Constant, "5"};
  inputRef = EntityRef{EntityType::Constant, "abc"};
  actual = transformer.transformArg(inputRef);
  REQUIRE(expected.getType() == actual.getType());
  REQUIRE(expected.getValue() == actual.getValue());
}