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
  UniqueVectorPtr<string> result = nullptr;
  try {
    result = qps->evaluate(query);
  } catch (const QPSParserSemanticError& ex) {
    FAIL("SEMANTIC ERROR");
  } catch (const QPSParserSyntaxError& ex) {
    FAIL("SYNTAX ERROR");
  }

  INFO("-----------------------------------------------\n")
  INFO("Result Size: " + to_string(result->size()) + "\n")
  string projectedResult = "";
  unordered_set<string> resultSet = unordered_set<string>();
  for (int i = 0; i < result->size(); i++) {
    projectedResult += result->at(i) + (i == result->size() - 1 ? "" : "  |  ");
    resultSet.insert(result->at(i));
  }

  INFO(projectedResult + "\n");
  INFO("-----------------------------------------------\n\n");
  assertSetEquality(answer, resultSet);
}
