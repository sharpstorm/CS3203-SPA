#include "catch.hpp"

#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "qps/projector/ResultProjector.h"
#include "../util/QueryResultTestUtil.cpp"
#include "../util/SynonymHolder.h"

using std::sort, std::unordered_set, std::vector, std::make_unique, std::unique_ptr;

string ERROR_MESSAGE = "Error";

vector<string> EXPECTED_ERROR = vector<string>({ERROR_MESSAGE});

void testResultProjection(vector<string>* expected, QPSOutputList *actual) {
  sort(expected->begin(), expected->end());
  vector<ProjectedValue> tempResult(actual->begin(), actual->end());
  sort(tempResult.begin(), tempResult.end());
  REQUIRE(*expected == tempResult);
}

SynonymHolder DECLARED_SYNS(
    {
        {PQL_SYN_TYPE_ASSIGN, "a"},
        {PQL_SYN_TYPE_VARIABLE, "v"}
    });
AttributedSynonym ATTR_TARGET_RESULT_VAR(DECLARED_SYNS.getProxy("a"));
AttributedSynonym ATTR_TARGET_ENTITY_VAR(DECLARED_SYNS.getProxy("v"));
AttributedSynonymList TARGET_RESULT_VARS{ATTR_TARGET_RESULT_VAR};
AttributedSynonymList TARGET_ENTITY_VARS{ATTR_TARGET_ENTITY_VAR};

TEST_CASE("Project when result is static") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  ProjectorResultTable result(false, true);
  QPSOutputList projectedResult;
  projector->project(&result, &TARGET_RESULT_VARS, &projectedResult);
  REQUIRE(projectedResult.empty());
}

TEST_CASE("Projecting Statements") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectorResultTable> result;
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"a", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3),
      }}
  }, &TARGET_RESULT_VARS);
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2", "3"}));
  projector->project(result.get(), &TARGET_RESULT_VARS, &actual);
  testResultProjection(expected.get(), &actual);
}

TEST_CASE("Projecting Entities") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectorResultTable> result;
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y")
      }}
  }, &TARGET_ENTITY_VARS);
  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  projector->project(result.get(), &TARGET_ENTITY_VARS, &actual);
  testResultProjection(expected.get(), &actual);
}
