#include "catch.hpp"
#include "qps/constraints/ConstantConstraint.h"

TEST_CASE("Constant Constraint True") {
  VariableTable varTable;
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  auto leftArg = make_unique<WithArgument>(1);
  auto rightArg = make_unique<WithArgument>(1);
  ConstantConstraint constraint(std::move(leftArg), std::move(rightArg));

  REQUIRE(constraint.validateConstraint());
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  leftArg = make_unique<WithArgument>("x");
  rightArg = make_unique<WithArgument>("x");
  constraint = ConstantConstraint(std::move(leftArg), std::move(rightArg));

  REQUIRE(constraint.validateConstraint());
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
}

TEST_CASE("Constant Constraint False") {
  VariableTable varTable;
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  auto leftArg = make_unique<WithArgument>(1);
  auto rightArg = make_unique<WithArgument>(2);
  ConstantConstraint constraint(std::move(leftArg), std::move(rightArg));

  REQUIRE(constraint.validateConstraint());
  REQUIRE_FALSE(constraint.applyConstraint(&builder, &overrides));

  leftArg = make_unique<WithArgument>("x");
  rightArg = make_unique<WithArgument>("y");
  constraint = ConstantConstraint(std::move(leftArg), std::move(rightArg));

  REQUIRE(constraint.validateConstraint());
  REQUIRE_FALSE(constraint.applyConstraint(&builder, &overrides));
}

TEST_CASE("Constant Incomparable Constraint") {
  VariableTable varTable;
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  auto leftArg = make_unique<WithArgument>(1);
  auto rightArg = make_unique<WithArgument>("x");
  ConstantConstraint constraint(std::move(leftArg), std::move(rightArg));

  REQUIRE_FALSE(constraint.validateConstraint());

  leftArg = make_unique<WithArgument>("y");
  rightArg = make_unique<WithArgument>(2);
  constraint = ConstantConstraint(std::move(leftArg), std::move(rightArg));

  REQUIRE_FALSE(constraint.validateConstraint());
}

