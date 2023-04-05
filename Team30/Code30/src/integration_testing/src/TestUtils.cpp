#include "catch.hpp"
#include "TestUtils.h"

void assertSetEquality(unordered_set<string> a, unordered_set<string> b) {
  REQUIRE(a.size() == b.size());
  for (const auto& elem: a) {
    REQUIRE(b.find(elem) != b.end());
  }
}

void launchQuery(IQPS* qps, string query, unordered_set<string> answer) {
  INFO("-----------------------------------------------\n");
  INFO("Query: " << query << "\n");
  QPSOutputList result;
  try {
    qps->evaluate(query, &result);
  } catch (const QPSParserSemanticError& ex) {
    FAIL("SEMANTIC ERROR");
  } catch (const QPSParserSyntaxError& ex) {
    FAIL("SYNTAX ERROR");
  }

  INFO("-----------------------------------------------\n")
  INFO("Result Size: " + to_string(result.size()) + "\n")
  string projectedResult = "";
  unordered_set<string> resultSet = unordered_set<string>();

  for (const ProjectedValue &item : result) {
    projectedResult += item;
    if (item != result.back()) {
      projectedResult += "  |  ";
    }
    resultSet.insert(item);
  }

  INFO(projectedResult + "\n");
  INFO("-----------------------------------------------\n\n");
  assertSetEquality(answer, resultSet);
}

void launchSyntaxErrorQuery(IQPS* qps, string query) {
  INFO("-----------------------------------------------\n");
  INFO("Query: " << query << "\n");
  QPSOutputList result;
  try {
    qps->evaluate(query, &result);
  } catch (const QPSParserSemanticError& ex) {
    FAIL("SEMANTIC ERROR");
  } catch (const QPSParserSyntaxError& ex) {
    return;
  }

  FAIL("SYNTAX ERROR SHOULD BE THROWN");
}

void launchSemanticErrorQuery(IQPS* qps, string query) {
  INFO("-----------------------------------------------\n");
  INFO("Query: " << query << "\n");
  QPSOutputList result;
  try {
    qps->evaluate(query, &result);
  } catch (const QPSParserSemanticError& ex) {
    return;
  } catch (const QPSParserSyntaxError& ex) {
    FAIL("SYNTAX ERROR");
  }

  FAIL("SEMANTIC ERROR SHOULD BE THROWN");
}
