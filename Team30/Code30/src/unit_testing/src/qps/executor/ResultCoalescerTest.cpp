#include "catch.hpp"

#include "qps/executor/ResultCoalescer.h"
#include "../util/QueryResultTestUtil.cpp"

TEST_CASE("Test QueryResult Coalesce - 1 same synonym") {
  auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          QueryResultItem(1),
          QueryResultItem(2),
          QueryResultItem(3)
      }},
      {"s2", QueryResultItemVector{
          QueryResultItem(2),
          QueryResultItem(3),
          QueryResultItem(4)
      }}
  });

  auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s2", QueryResultItemVector{
          QueryResultItem(1),
          QueryResultItem(2),
          QueryResultItem(3)
      }},
      {"v", QueryResultItemVector{
          QueryResultItem("x"),
          QueryResultItem("y"),
          QueryResultItem("z")
      }}
  });

  ResultCoalescer coalescer;
  auto result = PQLQueryResultPtr(coalescer.merge(resultA.get(), resultB.get()));
  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col).get()->project());
  }

  auto expected = unordered_set<string>{
      "y", "z"
  };

  resultA.release();
  resultB.release();

  REQUIRE(resultSet == expected);
}

TEST_CASE("Test QueryResult Coalesce - 2 same synonym") {
  auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          QueryResultItem(1),
          QueryResultItem(2),
          QueryResultItem(3),
          QueryResultItem(4)
      }},
      {"v", QueryResultItemVector{
          QueryResultItem("x"),
          QueryResultItem("y"),
          QueryResultItem("a"),
          QueryResultItem("z")
      }}
  });

  auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          QueryResultItem(1),
          QueryResultItem(2),
          QueryResultItem(3)
      }},
      {"v", QueryResultItemVector{
          QueryResultItem("x"),
          QueryResultItem("y"),
          QueryResultItem("z")
      }}
  });

  ResultCoalescer coalescer;
  auto result = PQLQueryResultPtr(coalescer.merge(resultA.get(), resultB.get()));
  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col).get()->project());
  }

  auto expected = unordered_set<string>{
      "x", "y"
  };

  resultA.release();
  resultB.release();

  REQUIRE(resultSet == expected);
}
