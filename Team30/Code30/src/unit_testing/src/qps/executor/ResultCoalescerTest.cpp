// #define ACTIVATE_BENCHMARK
#include "../../BenchmarkControl.h"
#include "catch.hpp"

#include "qps/executor/ResultCoalescer.h"
#include "../util/QueryResultTestUtil.cpp"

TEST_CASE("Test QueryResult Coalesce - 0 same synonym") {
  auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3)
      }},
      {"s2", QueryResultItemVector{
          TestResultItem(2),
          TestResultItem(3),
          TestResultItem(4)
      }}
  });

  auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s3", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3)
      }},
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y"),
          TestResultItem("z")
      }}
  });

  auto result = PQLQueryResultPtr(ResultCoalescer(resultA.get(), resultB.get())
                                      .merge());
  resultA.release();
  resultB.release();

  REQUIRE(result->isFalse());
}

TEST_CASE("Test QueryResult Coalesce - 1 same synonym") {
  auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3)
      }},
      {"s2", QueryResultItemVector{
          TestResultItem(2),
          TestResultItem(3),
          TestResultItem(4)
      }}
  });

  auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s2", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3)
      }},
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y"),
          TestResultItem("z")
      }}
  });

  auto result = PQLQueryResultPtr(ResultCoalescer(resultA.get(), resultB.get())
                                      .merge());
  resultA.release();
  resultB.release();

  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col)->project());
  }

  auto expected = unordered_set<string>{
      "y", "z"
  };

  REQUIRE(resultSet == expected);
}

TEST_CASE("Test QueryResult Coalesce - 2 same synonym") {
  auto resultA = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3),
          TestResultItem(4)
      }},
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y"),
          TestResultItem("a"),
          TestResultItem("z")
      }}
  });

  auto resultB = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"s1", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(2),
          TestResultItem(3)
      }},
      {"v", QueryResultItemVector{
          TestResultItem("x"),
          TestResultItem("y"),
          TestResultItem("z")
      }}
  });

  auto result = PQLQueryResultPtr(ResultCoalescer(resultA.get(), resultB.get())
                                      .merge());
  resultA.release();
  resultB.release();

  int col = result->getSynonymCol("v");

  unordered_set<string> resultSet;
  for (int i = 0; i < result->getRowCount(); i++) {
    resultSet.insert(result->getTableRowAt(i)->at(col)->project());
  }

  auto expected = unordered_set<string>{
      "x", "y"
  };

  REQUIRE(resultSet == expected);
}

#ifdef ACTIVATE_BENCHMARK
TEST_CASE("Benchmark Coalesce - 3x100 cross 3x1000 -> 5x100") {
  auto colA1 = QueryResultItemVector{};
  auto colA2 = QueryResultItemVector{};
  auto colA3 = QueryResultItemVector{};

  for (int i = 1; i <= 100; i++) {
    colA1.push_back(TestResultItem(i));
    colA2.push_back(TestResultItem("a" + i));
    colA3.push_back(TestResultItem("x" + i));
  }

  auto colB1 = QueryResultItemVector{};
  auto colB2 = QueryResultItemVector{};
  auto colB3 = QueryResultItemVector{};
  for (int i = 1000; i >= 1; i--) {
    colB1.push_back(TestResultItem(i));
    colB2.push_back(TestResultItem("a" + i));
    colB3.push_back(TestResultItem("x" + i));
  }

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
      auto result = PQLQueryResultPtr(ResultCoalescer(resultA.get(), resultB.get()).merge());
      resultA.release();
      resultB.release();
    };
}
#endif
