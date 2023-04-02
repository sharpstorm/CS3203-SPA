#include <list>
#include <string>

// #define ACTIVATE_BENCHMARK
#include "../../BenchmarkControl.h"
#include "catch.hpp"

#include "qps/executor/ResultCoalescer.h"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/projector/ResultProjector.h"
#include "../util/SynonymHolder.h"

using std::list, std::string;

#ifdef ACTIVATE_BENCHMARK
TEST_CASE("Benchmark Projection - 2x200 cross 2x200 cross 2x200 -> 4x1mil rows") {
  auto colA1 = QueryResultItemVector{};
  auto colA2 = QueryResultItemVector{};

  for (int i = 1; i <= 100; i++) {
    colA1.push_back(TestResultItem(i));
    colA2.push_back(TestResultItem("a" + i));
  }

  SynonymHolder syns({
                         {PQL_SYN_TYPE_ASSIGN, "a1"},
                         {PQL_SYN_TYPE_VARIABLE, "a2"},
                         {PQL_SYN_TYPE_ASSIGN, "b1"},
                         {PQL_SYN_TYPE_VARIABLE, "b2"},
                         {PQL_SYN_TYPE_ASSIGN, "c1"},
                         {PQL_SYN_TYPE_VARIABLE, "c2"}
                     });
  AttributedSynonymList targetVars{
      AttributedSynonym(syns.getProxy("a1")),
      AttributedSynonym(syns.getProxy("a2")),
      AttributedSynonym(syns.getProxy("b1")),
      AttributedSynonym(syns.getProxy("b2")),
      AttributedSynonym(syns.getProxy("c1")),
      AttributedSynonym(syns.getProxy("c2")),
  };

  auto result = TestQueryResultBuilder::buildExpectedTable(
      &targetVars,
      ExpectedParams{
          {"a1", colA1 },
          {"a2", colA2 },
      },
      ExpectedParams{
          {"b1", colA1 },
          {"b2", colA2 },
      },
      ExpectedParams{
          {"c1", colA1 },
          {"c2", colA2 },
      });

  list<string> output;

  PKB pkbStore;
  PkbQueryHandler pkbQH(&pkbStore);
  ResultProjector projector(&pkbQH);

  BENCHMARK("Benchmark 1 mil rows") -> void {
      QPSOutputList output;
      projector.project(result.get(), &targetVars, &output);
    };
}
#endif
