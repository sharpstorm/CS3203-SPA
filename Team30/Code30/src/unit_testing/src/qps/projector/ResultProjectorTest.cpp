#include "catch.hpp"

#include <vector>
#include <unordered_set>
#include <algorithm>

#include "qps/projector/ResultProjector.h"

using std::sort, std::unordered_set, std::vector;

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
  result.enitityPairs = pair_set<int, string>({ {1, "x"}, {1, "y"}});
  result.lines = unordered_set<int>({1, 2});
  result.entities = unordered_set<string>({"x", "y"});
  return result;
}

ResultProjector projector;

TEST_CASE("Project error result") {
  PQLQueryResult result;
  result.setError(ERROR_MESSAGE);

  vector<string>* projectedResult = projector.project(&result);
  REQUIRE(*projectedResult == EXPECTED_ERROR);
}


TEST_CASE("Project when result is static") {
  PQLQueryResult result;
  result.setIsStaticFalse(true);

  vector<string>* projectedResult = projector.project(&result);
  REQUIRE(projectedResult->empty());
}

TEST_CASE("Projecting Statement map") {
  PQLQueryResult result;
  vector<string>* actual;
  vector<string>* expected;

  // Projecting on Left arg
  // E.g. assign a; Select a such that Follows*(a,4);
  result.addToStatementMap("a", buildStatementResult(true));
  expected = new vector<string>({"1", "2", "3"});
  actual = projector.projectStatements(result.getStatementMap());
  testResultProjection(expected, actual);

  // Projecting on Right arg
  // E.g. assign a; Select a such that Follows*(1,a);
  result = PQLQueryResult();
  expected = new vector<string>({"2", "3", "4"});
  result.addToStatementMap("a", buildStatementResult(false));
  actual = projector.projectStatements(result.getStatementMap());
  testResultProjection(expected, actual);

  // Projecting when pairs empty
  // E.g. assign a; Select a
  StatementResult stmtResult;
  stmtResult.lines = unordered_set<int>({1, 2, 3, 4});
  result = PQLQueryResult();
  result.addToStatementMap("a", stmtResult);

  expected = new vector<string>({"1", "2", "3", "4"});
  actual = projector.projectStatements(result.getStatementMap());
  testResultProjection(expected, actual);
}

TEST_CASE("Projecting entity map") {
  PQLQueryResult result;
  EntityResult er;
  vector<string>* actual;
  vector<string>* expected;

  // Projecting on left arg
  // E.g. assign a; variable v; Select a such that Uses(a, v)
  result.addToEntityMap("a", buildEntityResult(true));

  expected = new vector<string>({"1", "1"});
  actual = projector.projectEntities(result.getEntityMap());
  testResultProjection(expected, actual);

  // Projecting on right arg
  // E.g. assign a; variable v; Select v such that Uses(a,v)
  result = PQLQueryResult();
  result.addToEntityMap("v", buildEntityResult(false));

  expected = new vector<string>({"x", "y"});
  actual = projector.projectEntities(result.getEntityMap());
  testResultProjection(expected, actual);

  // Projecting when pairs empty - Statement Type Synonym
  er = EntityResult() ;
  result = PQLQueryResult();
  er.lines = unordered_set<int>({1});
  result.addToEntityMap("a", er);

  expected = new vector<string>({"1"});
  actual = projector.projectEntities(result.getEntityMap());
  testResultProjection(expected, actual);

  // Projecting when pairs empty - Entity Type Synonym
  result = PQLQueryResult();
  er = EntityResult() ;
  er.entities = unordered_set<string>({"x", "y"});
  result.addToEntityMap("a", er);

  expected = new vector<string>({"x", "y"});
  actual = projector.projectEntities(result.getEntityMap());
  testResultProjection(expected, actual);
}
