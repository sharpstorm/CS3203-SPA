#include "catch.hpp"

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "qps/projector/ResultProjector.h"

using std::sort, std::unordered_set, std::vector, std::make_unique, std::unique_ptr;

string ERROR_MESSAGE = "Error";

vector<string> EXPECTED_ERROR = vector<string>({ERROR_MESSAGE});

void testResultProjection(vector<string>* expected, vector<string>* actual) {
  sort(expected->begin(), expected->end());
  sort(actual->begin(), actual->end());
  REQUIRE(*expected == *actual);
}

StatementResult buildStatementResult(bool leftArg) {
  StatementResult result;
  result.isLeftArg = leftArg;
  result.linePairs = pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}});
  result.lines = unordered_set<int>({1, 2, 3, 4});
  return result;
}

EntityResult buildEntityResult(bool leftArg) {
  EntityResult result;
  result.isLeftArg = leftArg;
  result.entityPairs = pair_set<int, string>({{1, "x"}, {1, "y"}});
  result.lines = unordered_set<int>({1, 2});
  result.entities = unordered_set<string>({"x", "y"});
  return result;
}

ResultProjector projector;
PQLQuerySynonym TARGET_RESULT_VAR{PQL_SYN_TYPE_ASSIGN, "a"};
PQLQuerySynonym TARGET_ENTITY_VAR{PQL_SYN_TYPE_VARIABLE, "v"};

TEST_CASE("Project error result") {
  PQLQueryResult result;
  result.setError(ERROR_MESSAGE);

  UniqueVectorPtr<string> projectedResult = projector.project(&result, TARGET_RESULT_VAR);
  REQUIRE(*projectedResult == EXPECTED_ERROR);
}


TEST_CASE("Project when result is static") {
  PQLQueryResult result;
  result.setIsStaticFalse(true);

  UniqueVectorPtr<string> projectedResult = projector.project(&result, TARGET_RESULT_VAR);
  REQUIRE(projectedResult->empty());
}

TEST_CASE("Projecting Statement map") {
  unique_ptr<PQLQueryResult> result;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  // Projecting on Left arg
  // E.g. assign a; Select a such that Follows*(a,4);
  result = make_unique<PQLQueryResult>();
  result->addToStatementMap("a", buildStatementResult(true));
  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2", "3"}));
  actual = projector.project(result.get(), TARGET_RESULT_VAR);
  testResultProjection(expected.get(), actual.get());

  // Projecting on Right arg
  // E.g. assign a; Select a such that Follows*(1,a);
  result = make_unique<PQLQueryResult>();
  result->addToStatementMap("a", buildStatementResult(false));
  expected = UniqueVectorPtr<string>(new vector<string>({"2", "3", "4"}));
  actual = projector.project(result.get(), TARGET_RESULT_VAR);
  testResultProjection(expected.get(), actual.get());

  // Projecting when pairs empty
  // E.g. assign a; Select a
  StatementResult stmtResult;
  stmtResult.lines = unordered_set<int>({1, 2, 3, 4});
  result = make_unique<PQLQueryResult>();make_unique<PQLQueryResult>();
  result->addToStatementMap("a", stmtResult);

  expected = UniqueVectorPtr<string>(new vector<string>({"1", "2", "3", "4"}));
  actual = projector.project(result.get(), TARGET_RESULT_VAR);
  testResultProjection(expected.get(), actual.get());
}

TEST_CASE("Projecting entity map") {
  unique_ptr<PQLQueryResult> result;
  EntityResult er;
  UniqueVectorPtr<string> actual;
  UniqueVectorPtr<string> expected;

  // Projecting on left arg
  // E.g. assign a; variable v; Select a such that Uses(a, v)
  result = make_unique<PQLQueryResult>();
  result->addToEntityMap("a", buildEntityResult(true));

  expected = UniqueVectorPtr<string>(new vector<string>({"1", "1"}));
  actual = projector.project(result.get(), TARGET_RESULT_VAR);
  testResultProjection(expected.get(), actual.get());

  // Projecting on right arg
  // E.g. assign a; variable v; Select v such that Uses(a,v)
  result = make_unique<PQLQueryResult>();
  result->addToEntityMap("v", buildEntityResult(false));

  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  actual = projector.project(result.get(), TARGET_ENTITY_VAR);
  testResultProjection(expected.get(), actual.get());

  // Projecting when pairs empty - Statement Type Synonym
  er = EntityResult();
  result = make_unique<PQLQueryResult>();
  er.lines = unordered_set<int>({1});
  result->addToEntityMap("a", er);

  expected = UniqueVectorPtr<string>(new vector<string>({"1"}));
  actual = projector.project(result.get(), TARGET_RESULT_VAR);
  testResultProjection(expected.get(), actual.get());

  // Projecting when pairs empty - Entity Type Synonym
  result = make_unique<PQLQueryResult>();
  er = EntityResult() ;
  er.entities = unordered_set<string>({"x", "y"});
  result->addToEntityMap("v", er);

  expected = UniqueVectorPtr<string>(new vector<string>({"x", "y"}));
  actual = projector.project(result.get(), TARGET_ENTITY_VAR);
  testResultProjection(expected.get(), actual.get());
}
