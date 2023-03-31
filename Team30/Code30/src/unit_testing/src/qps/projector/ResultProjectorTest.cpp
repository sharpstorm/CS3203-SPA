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

PQLQuerySynonym TARGET_RESULT_RAW{PQL_SYN_TYPE_ASSIGN, "a"};
PQLQuerySynonym TARGET_ENTITY_RAW{PQL_SYN_TYPE_VARIABLE, "v"};
PQLQuerySynonym* TARGET_RESULT_PTR = &TARGET_RESULT_RAW;
PQLQuerySynonym* TARGET_ENTITY_PTR = &TARGET_ENTITY_RAW;
PQLQuerySynonymProxy TARGET_RESULT_VAR(&TARGET_RESULT_PTR);
PQLQuerySynonymProxy TARGET_ENTITY_VAR(&TARGET_ENTITY_PTR);

AttributedSynonym ATTR_TARGET_RESULT_VAR(TARGET_RESULT_VAR);
AttributedSynonym ATTR_TARGET_ENTITY_VAR(TARGET_ENTITY_VAR);
AttributedSynonymList TARGET_RESULT_VARS{ATTR_TARGET_RESULT_VAR};
AttributedSynonymList TARGET_ENTITY_VARS{ATTR_TARGET_ENTITY_VAR};

TEST_CASE("Project when result is static") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  SynonymResultTable result(false, true);
  UniqueVectorPtr<string> projectedResult = projector->project(&result, &TARGET_RESULT_VARS);
  REQUIRE(projectedResult->empty());
}

TEST_CASE("Projecting Statements") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<SynonymResultTable> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"a", QueryResultItemVector{
        TestResultItem(1),
        TestResultItem(2),
        TestResultItem(3),
      }}
  }, &TARGET_RESULT_VARS);
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2", "3"}));
  actual = projector->project(result.get(), &TARGET_RESULT_VARS);
  testResultProjection(expected.get(), actual.get());
}

TEST_CASE("Projecting Entities") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<SynonymResultTable> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y")
      }}
  }, &TARGET_ENTITY_VARS);
  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  actual = projector->project(result.get(), &TARGET_ENTITY_VARS);
  testResultProjection(expected.get(), actual.get());
}
