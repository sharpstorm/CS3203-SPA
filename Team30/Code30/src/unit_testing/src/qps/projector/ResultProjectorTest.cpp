#include "catch.hpp"

#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "qps/projector/ResultProjector.h"
#include "../util/QueryResultTestUtil.cpp"


using std::sort, std::unordered_set, std::vector, std::make_unique, std::unique_ptr;

string ERROR_MESSAGE = "Error";

vector<string> EXPECTED_ERROR = vector<string>({ERROR_MESSAGE});

void testResultProjection(vector<string>* expected, vector<string>* actual) {
  sort(expected->begin(), expected->end());
  sort(actual->begin(), actual->end());
  REQUIRE(*expected == *actual);
}

ResultProjector projector;
PQLQuerySynonym TARGET_RESULT_VAR{PQL_SYN_TYPE_ASSIGN, "a"};
PQLQuerySynonym TARGET_ENTITY_VAR{PQL_SYN_TYPE_VARIABLE, "v"};
PQLQuerySynonymList TARGET_RESULT_VARS{TARGET_RESULT_VAR};
PQLQuerySynonymList TARGET_ENTITY_VARS{TARGET_ENTITY_VAR};

TEST_CASE("Project when result is static") {
  PQLQueryResult result;
  result.setIsStaticFalse(true);

  UniqueVectorPtr<string> projectedResult = projector.project(&result, &TARGET_RESULT_VARS);
  REQUIRE(projectedResult->empty());
}

TEST_CASE("Projecting Statements") {
  unique_ptr<PQLQueryResult> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
        QueryResultItem(1),
        QueryResultItem(2),
        QueryResultItem(3),
      }}
  });
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2", "3"}));
  actual = projector.project(result.get(), &TARGET_RESULT_VARS);
  testResultProjection(expected.get(), actual.get());
}

TEST_CASE("Projecting Entities") {
  unique_ptr<PQLQueryResult> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"v", QueryResultItemVector{
          QueryResultItem("x"),
          QueryResultItem("y")
      }}
  });
  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  actual = projector.project(result.get(), &TARGET_ENTITY_VARS);
  testResultProjection(expected.get(), actual.get());
}
