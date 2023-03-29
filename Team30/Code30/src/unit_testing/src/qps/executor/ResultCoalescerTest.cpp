// #define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

#include "qps/executor/ResultCoalescer.h"
#include "../util/QueryResultTestUtil.cpp"

TEST_CASE("Test QueryResult Coalesce - 0 same synonym") {
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
      {"s3", QueryResultItemVector{
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
  resultA.release();
  resultB.release();

  REQUIRE(result->isFalse());
}

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
  resultA.release();
  resultB.release();

  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col).get()->project());
  }

  auto expected = unordered_set<string>{
      "y", "z"
  };

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
  resultA.release();
  resultB.release();

  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col).get()->project());
  }

  auto expected = unordered_set<string>{
      "x", "y"
  };

  REQUIRE(resultSet == expected);
}

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING
TEST_CASE("Benchmark Coalesce - 3x100 cross 3x1000 -> 5x100") {
  auto colA1 = QueryResultItemVector{};
  auto colA2 = QueryResultItemVector{};
  auto colA3 = QueryResultItemVector{};

  for (int i = 1; i <= 100; i++) {
    colA1.push_back(QueryResultItem(i));
    colA2.push_back(QueryResultItem("a" + i));
    colA3.push_back(QueryResultItem("x" + i));
  }

  auto colB1 = QueryResultItemVector{};
  auto colB2 = QueryResultItemVector{};
  auto colB3 = QueryResultItemVector{};
  for (int i = 1000; i >= 1; i--) {
    colB1.push_back(QueryResultItem(i));
    colB2.push_back(QueryResultItem("a" + i));
    colB3.push_back(QueryResultItem("x" + i));
  }

  ResultCoalescer coalescer;
  BENCHMARK("Benchmark 100 coalesce") -> void {
      auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
          {"a1", colA1 },
          {"a2", colA2 },
          {"common", colA3 },
      });

      auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
          {"b1", colB1 },
          {"b2", colB2 },
          {"common", colB3 },
      });
      auto result = PQLQueryResultPtr(coalescer.merge(resultA.get(), resultB.get()));
      resultA.release();
      resultB.release();
    };
}
#endif
