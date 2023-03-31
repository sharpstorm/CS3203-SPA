#include <memory>

#include "catch.hpp"
#include "common/pattern/PatternConverter.h"
#include "TestASTProvider.h"

using std::make_shared, std::make_unique;

TEST_CASE("ExpressionSequence Conversion - Balanced") {
//  TestASTProvider treeProvider;
//  auto expr = PatternConverter::convertASTToPostfix(
//      treeProvider.balancedTree.get());
//
//  // x * y + x * z;
//  ExpressionSequence expected{ "x", "y", "*", "x", "z", "*", "+" };
//  REQUIRE(*expr == expected);
}

TEST_CASE("ExpressionSequence Conversion - Right Heavy") {
//  TestASTProvider treeProvider;
//  auto expr = PatternConverter::convertASTToPostfix(
//      treeProvider.rightHeavyTree.get());
//
//  // x + y * z;
//  ExpressionSequence expected{ "x", "y", "z", "*", "+" };
//  REQUIRE(*expr == expected);
}

TEST_CASE("ExpressionSequence Conversion - Left Heavy") {
//  TestASTProvider treeProvider;
//  auto expr = PatternConverter::convertASTToPostfix(
//      treeProvider.leftHeavyTree.get());
//
//  // x * y + z;
//  ExpressionSequence expected{ "x", "y", "*", "z", "+" };
//  REQUIRE(*expr == expected);
}
