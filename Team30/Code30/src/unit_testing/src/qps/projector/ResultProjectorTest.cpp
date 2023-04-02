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
PQLQuerySynonym TARGET_RESULT_RAW2{PQL_SYN_TYPE_ASSIGN, "a2"};
PQLQuerySynonym TARGET_ENTITY_RAW{PQL_SYN_TYPE_VARIABLE, "v"};
PQLQuerySynonym TARGET_ENTITY_RAW2{PQL_SYN_TYPE_VARIABLE, "v2"};

PQLQuerySynonym* TARGET_RESULT_PTR = &TARGET_RESULT_RAW;
PQLQuerySynonym* TARGET_RESULT_PTR2 = &TARGET_RESULT_RAW2;
PQLQuerySynonym* TARGET_ENTITY_PTR = &TARGET_ENTITY_RAW;
PQLQuerySynonym* TARGET_ENTITY_PTR2 = &TARGET_ENTITY_RAW2;

PQLQuerySynonymProxy TARGET_RESULT_VAR(&TARGET_RESULT_PTR);
PQLQuerySynonymProxy TARGET_RESULT_VAR2(&TARGET_RESULT_PTR2);
PQLQuerySynonymProxy TARGET_ENTITY_VAR(&TARGET_ENTITY_PTR);
PQLQuerySynonymProxy TARGET_ENTITY_VAR2(&TARGET_ENTITY_PTR2);

AttributedSynonym ATTR_TARGET_RESULT_VAR(TARGET_RESULT_VAR);
AttributedSynonym ATTR_TARGET_RESULT_VAR2(TARGET_RESULT_VAR2);
AttributedSynonym ATTR_TARGET_ENTITY_VAR(TARGET_ENTITY_VAR);
AttributedSynonym ATTR_TARGET_ENTITY_VAR2(TARGET_ENTITY_VAR2);
AttributedSynonymList TARGET_RESULT_VARS{ATTR_TARGET_RESULT_VAR};
AttributedSynonymList TARGET_ENTITY_VARS{ATTR_TARGET_ENTITY_VAR};

AttributedSynonymList TARGET_TUPLE_STMT_VAR{ATTR_TARGET_RESULT_VAR, ATTR_TARGET_RESULT_VAR2};
AttributedSynonymList TARGET_TUPLE_ENT_VAR{ATTR_TARGET_ENTITY_VAR, ATTR_TARGET_ENTITY_VAR2};
AttributedSynonymList TARGET_TUPLE_DIFF_TYPE{ATTR_TARGET_RESULT_VAR, ATTR_TARGET_ENTITY_VAR};

TEST_CASE("Projection on BOOLEAN") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;
  unique_ptr<SynonymResultTable> result;
  vector<AttributedSynonym> emptyAttr = vector<AttributedSynonym>();

  result = make_unique<SynonymResultTable>(true, true);
  expected = UniqueVectorPtr<string>(new vector<string>({"TRUE"}));
  actual = projector->project(result.get(), &emptyAttr);
  testResultProjection(expected.get(), actual.get());

  result = make_unique<SynonymResultTable>(true, false);
  expected = UniqueVectorPtr<string>(new vector<string>({"FALSE"}));
  actual = projector->project(result.get(), &emptyAttr);
  testResultProjection(expected.get(), actual.get());
}

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

TEST_CASE("Projecting Tuples - Same Types - Same Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<SynonymResultTable> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
        TestResultItem("x"), TestResultItem("y")}},
      {"v2", QueryResultItemVector{TestResultItem("x"), TestResultItem("y")}}
  }, &TARGET_TUPLE_ENT_VAR);
  expected = UniqueVectorPtr<string>(new vector<string>({"x x", "y y"}));
  actual = projector->project(result.get(), &TARGET_TUPLE_ENT_VAR);
  testResultProjection(expected.get(), actual.get());

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"a", QueryResultItemVector{
          TestResultItem(1), TestResultItem(2), TestResultItem(3),
      }}, {"a2", QueryResultItemVector{
           TestResultItem(1), TestResultItem(2), TestResultItem(3)}}
  }, &TARGET_TUPLE_STMT_VAR);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 1", "2 2", "3 3"}));
  actual = projector->project(result.get(), &TARGET_TUPLE_STMT_VAR);
  testResultProjection(expected.get(), actual.get());
}

TEST_CASE("Projecting Tuples - Different Types - Same Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<SynonymResultTable> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
          TestResultItem("x"), TestResultItem("y"), TestResultItem("z")}},
      {"a", QueryResultItemVector{
          TestResultItem(1), TestResultItem(2), TestResultItem(3)}}
  }, &TARGET_TUPLE_DIFF_TYPE);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 x","2 y", "3 z"}));
  actual = projector->project(result.get(), &TARGET_TUPLE_DIFF_TYPE);
  testResultProjection(expected.get(), actual.get());
}

TEST_CASE("Projecting Tuples - Different Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<SynonymResultTable> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  vector<ExpectedParams> expectedGroups({
    ExpectedParams{{"v", QueryResultItemVector{
      TestResultItem("x"), TestResultItem("y"), TestResultItem("z")}}},
    ExpectedParams{{"a", QueryResultItemVector{
      TestResultItem(1), TestResultItem(2), TestResultItem(3)}}}
  });
  vector<AttributedSynonymList*> synList({&TARGET_ENTITY_VARS, &TARGET_RESULT_VARS});

  result = TestQueryResultBuilder::buildExpectedTable(expectedGroups, synList);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 x", "2 x", "3 x",
                                                         "1 y", "2 y", "3 y",
                                                         "1 z", "2 z", "3 z"}));
  actual = projector->project(result.get(), &TARGET_TUPLE_DIFF_TYPE);
  testResultProjection(expected.get(), actual.get());
}