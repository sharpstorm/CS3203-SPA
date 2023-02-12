#include "catch.hpp"
#include <unordered_set>

#include "qps/common/PQLQueryResult.h"

using std::unordered_set;

PQL_VAR_NAME TEST_VAR_NAME = "a";
PQL_VAR_NAME TEST_NOT_EXIST_VAR_NAME = "z";

pair_set<int, int> LINE_PAIRS = pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}});
unordered_set<int> STATEMENT_LINES = unordered_set<int>({1, 2, 3, 4});

pair_set<int, string> ENTITY_PAIRS = pair_set<int, string>({ {1, "x"}, {1, "y"}});
unordered_set<int> ENTITY_LINES = unordered_set<int>({1, 2});
unordered_set<string> ENTITIES = unordered_set<string>({"x", "y"});

StatementResult buildStatementResult() {
  StatementResult result;
  result.isLeftArg = true;
  result.linePairs = LINE_PAIRS;
  result.lines = STATEMENT_LINES;
  return result;
}

EntityResult buildEntityResult() {
  EntityResult result;
  result.isLeftArg = true;
  result.entityPairs = ENTITY_PAIRS;
  result.lines = ENTITY_LINES;
  result.entities = ENTITIES;
  return result;
}

TEST_CASE("Add to StatementMap") {
  PQLQueryResult result;

  result.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
  STATEMENT_MAP statementMap = result.getStatementMap();

  REQUIRE(statementMap.find(TEST_VAR_NAME) != statementMap.end());

  StatementResult stmtResult = statementMap[TEST_VAR_NAME];
  REQUIRE(stmtResult == buildStatementResult());
}

TEST_CASE("Add to EntityMap") {
  PQLQueryResult result;

  result.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
  ENTITY_MAP entityMap = result.getEntityMap();

  REQUIRE(entityMap.find(TEST_VAR_NAME) != entityMap.end());

  EntityResult entityResult = entityMap[TEST_VAR_NAME];
    REQUIRE(entityResult == buildEntityResult());
}

TEST_CASE("StatementMap Emptiness Check") {
  PQLQueryResult result;

  REQUIRE(result.isStatementMapEmpty() == true);

  result.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
  REQUIRE(result.isStatementMapEmpty() == false);
}

TEST_CASE("EntityMap Emptiness Check") {
  PQLQueryResult result;

  REQUIRE(result.isEntityMapEmpty() == true);

  result.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
  REQUIRE(result.isEntityMapEmpty() == false);
}

TEST_CASE("Retrieve StatementResult") {
  PQLQueryResult result;

  result.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
  REQUIRE(result.getFromStatementMap(TEST_VAR_NAME) != nullptr);
  REQUIRE(result.getFromStatementMap(TEST_NOT_EXIST_VAR_NAME) == nullptr);
}

TEST_CASE("Retrieve EntityResult") {
  PQLQueryResult result;

  result.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
  REQUIRE(result.getFromEntityMap(TEST_VAR_NAME) != nullptr);
  REQUIRE(result.getFromEntityMap(TEST_NOT_EXIST_VAR_NAME) == nullptr);
}

//TEST_CASE("Retrieve Variable from Static Result") {
//  PQLQueryResult result;
//  PQLQueryResult expected;
//  expected.setIsStaticFalse(true);
//  PQLQueryVariable queryVariable{PQL_VAR_TYPE_ASSIGN, TEST_VAR_NAME};
//
//  // Static Result E.g. Follows(1,2)
//  result.setIsStaticFalse(true);
//  REQUIRE(*(result.filterResultTo(queryVariable)) == expected);
//}

//TEST_CASE("Retrieve Variable from Statement Result") {
//  PQLQueryResult expected;
//  PQLQueryResult actual;
//  PQLQueryResult result =  PQLQueryResult();
//  PQLQueryVariable queryVariable = {PQL_VAR_TYPE_ASSIGN, TEST_NOT_EXIST_VAR_NAME};
//
//  // Statement Result with target variable not in map
//  // E.g. assign a1, a2; Select a2 such that Follows(a1, 4)
//  result.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
//  actual = *(result.filterResultTo(queryVariable));
//  REQUIRE(actual == expected);
//
//  // Statement Result with target variable in map
//  // E.g. assign a; Select a such that Follows(a, 2)
//  queryVariable = {PQL_VAR_TYPE_ASSIGN, TEST_VAR_NAME};
//  expected.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
//  actual = *(result.filterResultTo(queryVariable));
//  REQUIRE(actual == expected);
//}

//TEST_CASE("Retrieve Variable from Entity Result") {
//  PQLQueryResult expected;
//  PQLQueryResult actual;
//  PQLQueryResult result = PQLQueryResult();
//  PQLQueryVariable query_variable = {PQL_VAR_TYPE_VARIABLE, TEST_NOT_EXIST_VAR_NAME};
//
//  // Entity Result with target variable not in map
//  // E.g. variable v1, v2; Select v2 such that Uses(3, v1)
//  result.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
//  actual = *(result.filterResultTo(query_variable));
//  REQUIRE(actual == expected);
//
//  // Entity Result with target variable in map
//  // E.g. variable v; Select v such that Uses(a, v)
//  query_variable = {PQL_VAR_TYPE_VARIABLE, TEST_VAR_NAME};
//  expected.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
//  actual = *(result.filterResultTo(query_variable));
//  REQUIRE(actual == expected);
//}
