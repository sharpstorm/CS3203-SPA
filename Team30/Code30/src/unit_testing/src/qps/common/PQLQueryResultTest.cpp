#include "catch.hpp"
#include <vector>

#include "qps/common/PQLQueryResult.h"

using std::vector;

StatementResult buildStatementResult() {
  StatementResult result;
  result.isLeftArg = true;
  result.linePairs = vector<pair<int, int>>({{1, 2}, {2, 3}, {3, 4}});
  result.lines = vector<int>({1, 2, 3, 4});
  return result;
}

EntityResult buildEntityResult() {
  EntityResult result;
  result.isLeftArg = true;
  result.enitityPairs = vector<pair<int, string>>({ {1, "x"}, {1, "y"}});
  result.lines = vector<int>({1, 2});
  result.entities = vector<string>({"x", "y"});
  return result;
}

bool isStatementResultSame(StatementResult a, StatementResult b) {
  bool isLeftArgEqual = a.isLeftArg == b.isLeftArg;
  bool isLinesEqual = a.lines == b.lines;
  bool isLinePairsEqual = a.linePairs == b.linePairs;
  return isLeftArgEqual && isLinesEqual && isLinePairsEqual;
}

bool isEntityResultSame(EntityResult a, EntityResult b) {
  bool isLeftArgEqual = a.isLeftArg == b.isLeftArg;
  bool isLinesEqual = a.lines == b.lines;
  bool isEntitiesEqual = a.entities == b.entities;
  bool isEntityPairsEqual = a.enitityPairs == b.enitityPairs;
  return isLeftArgEqual && isLinesEqual && isEntitiesEqual &&
      isEntityPairsEqual;
}

PQL_VAR_NAME TEST_VAR_NAME = "a";
PQL_VAR_NAME TEST_NOT_EXIST_VAR_NAME = "z";

TEST_CASE("Add to StatementMap") {
  PQLQueryResult result;

  result.addToStatementMap(TEST_VAR_NAME, buildStatementResult());
  STATEMENT_MAP statementMap = result.getStatementMap();

  REQUIRE(statementMap.find(TEST_VAR_NAME) != statementMap.end());

  StatementResult stmtResult = statementMap[TEST_VAR_NAME];
  REQUIRE(isStatementResultSame(stmtResult, buildStatementResult()) == true);
}

TEST_CASE("Add to EntityMap") {
  PQLQueryResult result;

  result.addToEntityMap(TEST_VAR_NAME, buildEntityResult());
  ENTITY_MAP entityMap = result.getEntityMap();

  REQUIRE(entityMap.find(TEST_VAR_NAME) != entityMap.end());

  EntityResult entityResult = entityMap[TEST_VAR_NAME];
  REQUIRE(isEntityResultSame(entityResult, buildEntityResult()));
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