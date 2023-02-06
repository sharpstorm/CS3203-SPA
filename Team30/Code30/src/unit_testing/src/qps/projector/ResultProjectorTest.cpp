#include "catch.hpp"

#include "qps/projector/ResultProjector.h"

string ERROR_MESSAGE = "Error";

ResultProjector projector;

TEST_CASE("Project error result") {
  PQLQueryResult result;
  result.setError(ERROR_MESSAGE);

  string* projectedResult = projector.project(&result);
  REQUIRE(projectedResult->compare(ERROR_MESSAGE) == 0);
}

TEST_CASE("Project when result is static") {
  PQLQueryResult result;
  result.setIsStaticTrue(true);

  string* projectedResult = projector.project(&result);
  REQUIRE(projectedResult->compare("") == 0);
}

TEST_CASE("Projecting with empty statement and entity maps") {
  PQLQueryResult result;

  string* projectedResult = projector.project(&result);
  REQUIRE(projectedResult->compare("") == 0);
}

//TEST_CASE("Projecting on statement map") {
////  Left Arg
////  Right Arg
//}
//
//TEST_CASE("Projecting on entity map") {
// // Left Arg
// // Right Arg
//}