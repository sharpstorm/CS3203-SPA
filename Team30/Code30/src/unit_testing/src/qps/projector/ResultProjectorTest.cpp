#include "catch.hpp"

#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "qps/projector/ResultProjector.h"
#include "../util/QueryResultTestUtil.cpp"
#include "../util/SynonymHolder.h"
#include "ProjectorPKBStub.cpp"

using std::sort, std::unordered_set, std::vector, std::make_unique, std::unique_ptr;

string ERROR_MESSAGE = "Error";

vector<string> EXPECTED_ERROR = vector<string>({ERROR_MESSAGE});

void testResultProjection(vector<string>* expected, QPSOutputList *actual) {
  sort(expected->begin(), expected->end());
  vector<ProjectedValue> tempResult(actual->begin(), actual->end());
  sort(tempResult.begin(), tempResult.end());
  REQUIRE(*expected == tempResult);
}

void testAttributeProjection(ResultProjector* projector,
                             AttributedSynonym* syn,
                             ExpectedParams* params,
                             vector<string>* expected) {
  QPSOutputList actual;
  AttributedSynonymList synList = AttributedSynonymList({*syn});
  auto result = TestQueryResultBuilder::buildExpectedTable(*params, &synList);
  projector->project(result.get(), &synList, &actual);
  testResultProjection(expected, &actual);
}

SynonymHolder DECLARED_SYNS(
    {
        {PQL_SYN_TYPE_ASSIGN, "a"},
        {PQL_SYN_TYPE_VARIABLE, "v"},
        {PQL_SYN_TYPE_ASSIGN, "a2"},
        {PQL_SYN_TYPE_VARIABLE, "v2"},
        {PQL_SYN_TYPE_CALL, "cl"},
        {PQL_SYN_TYPE_PROCEDURE, "p"},
        {PQL_SYN_TYPE_CONSTANT, "c"},
        {PQL_SYN_TYPE_PRINT, "pn"},
        {PQL_SYN_TYPE_READ, "r"}
    });
AttributedSynonym ATTR_TARGET_RESULT_VAR(DECLARED_SYNS.getProxy("a"));
AttributedSynonym ATTR_TARGET_RESULT_VAR2(DECLARED_SYNS.getProxy("a2"));
AttributedSynonym ATTR_TARGET_ENTITY_VAR(DECLARED_SYNS.getProxy("v"));
AttributedSynonym ATTR_TARGET_ENTITY_VAR2(DECLARED_SYNS.getProxy("v2"));
AttributedSynonymList TARGET_RESULT_VARS{ATTR_TARGET_RESULT_VAR};
AttributedSynonymList TARGET_ENTITY_VARS{ATTR_TARGET_ENTITY_VAR};

AttributedSynonymList TARGET_TUPLE_STMT_VAR{ATTR_TARGET_RESULT_VAR, ATTR_TARGET_RESULT_VAR2};
AttributedSynonymList TARGET_TUPLE_ENT_VAR{ATTR_TARGET_ENTITY_VAR, ATTR_TARGET_ENTITY_VAR2};
AttributedSynonymList TARGET_TUPLE_DIFF_TYPE{ATTR_TARGET_RESULT_VAR, ATTR_TARGET_ENTITY_VAR};

TEST_CASE("Projection on BOOLEAN") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;
  unique_ptr<ProjectableTable> result;
  vector<AttributedSynonym> emptyAttr = vector<AttributedSynonym>();

  result = make_unique<ProjectableTable>(true, true);
  expected = UniqueVectorPtr<string>(new vector<string>({"TRUE"}));
  projector->project(result.get(), &emptyAttr, &actual);
  testResultProjection(expected.get(), &actual);

  actual = list<ProjectedValue>();
  result = make_unique<ProjectableTable>(true, false);
  expected = UniqueVectorPtr<string>(new vector<string>({"FALSE"}));
  projector->project(result.get(), &emptyAttr, &actual);
  testResultProjection(expected.get(), &actual);
}

TEST_CASE("Project when result is static") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  ProjectableTable result(false, true);
  QPSOutputList projectedResult;
  projector->project(&result, &TARGET_RESULT_VARS, &projectedResult);
  REQUIRE(projectedResult.empty());
}

TEST_CASE("Projecting Statements") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectableTable> result;
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
  unique_ptr<ProjectableTable> result;
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

TEST_CASE("Projecting Attributes") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<ProjectorPKBStub>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  UniqueVectorPtr<string> expected;

  // stmt#
  AttributedSynonym syn = AttributedSynonym(DECLARED_SYNS.getProxy("a"), STMT_NUM);
  auto params = ExpectedParams{{"a", QueryResultItemVector{TestResultItem(1), TestResultItem(2)}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  // varName
  syn = AttributedSynonym(DECLARED_SYNS.getProxy("v"), VAR_NAME);
  params =ExpectedParams{{"v", QueryResultItemVector{TestResultItem("x"), TestResultItem("y")}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  syn = AttributedSynonym(DECLARED_SYNS.getProxy("r"), VAR_NAME);
  params = ExpectedParams{{"r", QueryResultItemVector{TestResultItem(1), TestResultItem(2)}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"readVar", "readVar"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  syn = AttributedSynonym(DECLARED_SYNS.getProxy("pn"), VAR_NAME);
  params = ExpectedParams{{"pn", QueryResultItemVector{TestResultItem(1), TestResultItem(2)}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"printVar", "printVar"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  // procName
  syn = AttributedSynonym(DECLARED_SYNS.getProxy("p"), PROC_NAME);
  params = ExpectedParams{{"p", QueryResultItemVector{TestResultItem("x"), TestResultItem("y")}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  syn = AttributedSynonym(DECLARED_SYNS.getProxy("cl"), PROC_NAME);
  params = ExpectedParams{{"cl", QueryResultItemVector{TestResultItem(1), TestResultItem(2)}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"callProc", "callProc"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());

  // value
  syn = AttributedSynonym(DECLARED_SYNS.getProxy("c"), CONST_VALUE);
  params = ExpectedParams{{"c", QueryResultItemVector{TestResultItem(1), TestResultItem(2)}}};
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2"}));
  testAttributeProjection(projector.get(), &syn, &params, expected.get());
}

TEST_CASE("Projecting Tuples - Same Types - Same Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectableTable> result;
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
        TestResultItem("x"), TestResultItem("y")}},
      {"v2", QueryResultItemVector{TestResultItem("x"), TestResultItem("y")}}
  }, &TARGET_TUPLE_ENT_VAR);
  expected = UniqueVectorPtr<string>(new vector<string>({"x x", "y y"}));
  projector->project(result.get(), &TARGET_TUPLE_ENT_VAR, &actual);
  testResultProjection(expected.get(), &actual);

  actual = list<ProjectedValue>();
  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"a", QueryResultItemVector{
          TestResultItem(1), TestResultItem(2), TestResultItem(3),
      }}, {"a2", QueryResultItemVector{
           TestResultItem(1), TestResultItem(2), TestResultItem(3)}}
  }, &TARGET_TUPLE_STMT_VAR);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 1", "2 2", "3 3"}));
  projector->project(result.get(), &TARGET_TUPLE_STMT_VAR, &actual);
  testResultProjection(expected.get(), &actual);
}

TEST_CASE("Projecting Tuples - Different Types - Same Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectableTable> result;
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;

  result = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
      {"v", QueryResultItemVector{
          TestResultItem("x"), TestResultItem("y"), TestResultItem("z")}},
      {"a", QueryResultItemVector{
          TestResultItem(1), TestResultItem(2), TestResultItem(3)}}
  }, &TARGET_TUPLE_DIFF_TYPE);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 x","2 y", "3 z"}));
  projector->project(result.get(), &TARGET_TUPLE_DIFF_TYPE, &actual);
  testResultProjection(expected.get(), &actual);
}

TEST_CASE("Projecting Tuples - Different Group") {
  PKB pkbStore;
  unique_ptr<PkbQueryHandler> pkbQH = make_unique<PkbQueryHandler>(&pkbStore);
  unique_ptr<ResultProjector> projector = make_unique<ResultProjector>(pkbQH.get());
  unique_ptr<ProjectableTable> result;
  QPSOutputList actual;
  UniqueVectorPtr<string> expected;

  vector<ExpectedParams> expectedGroups({
    ExpectedParams{{"v", QueryResultItemVector{
      TestResultItem("x"), TestResultItem("y"), TestResultItem("z")}}},
    ExpectedParams{{"a", QueryResultItemVector{
      TestResultItem(1), TestResultItem(2), TestResultItem(3)}}}
  });
//  vector<AttributedSynonymList*> synList({&TARGET_ENTITY_VARS, &TARGET_RESULT_VARS});

  result = TestQueryResultBuilder::buildExpectedTable(expectedGroups);
  expected = UniqueVectorPtr<string>(new vector<string>({"1 x", "2 x", "3 x",
                                                         "1 y", "2 y", "3 y",
                                                         "1 z", "2 z", "3 z"}));
  projector->project(result.get(), &TARGET_TUPLE_DIFF_TYPE, &actual);
  testResultProjection(expected.get(), &actual);
}
