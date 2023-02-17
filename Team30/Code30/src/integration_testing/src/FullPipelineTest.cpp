#include "catch.hpp"

#include <memory>
#include <unordered_set>
#include "common/UtilityTypes.h"
#include "qps/QPSFacade.h"
#include "pkb/writers/PkbWriter.h"
#include "qps/errors/QPSError.h"
#include "sp/SpDriver.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

void assertSetEquality2(unordered_set<string> a, unordered_set<string> b) {
  REQUIRE(a.size() == b.size());
  for (const auto& elem: a) {
    REQUIRE(b.find(elem) != b.end());
  }
}

void launchQuery2(IQPS* qps, string query, unordered_set<string> answer) {
  INFO("-----------------------------------------------\n");
  INFO("Query: " << query << "\n");
  UniqueVectorPtr<string> result = nullptr;
  try {
    result = qps->evaluate(query);
  } catch (const QPSError& ex) {
    FAIL(ex.what());
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
  assertSetEquality2(answer, resultSet);
}

TEST_CASE("Test Full End-to-end") {
  auto pkb = make_unique<PKB>();
  auto pkbWriter = make_unique<PkbWriter>(pkb.get());
  shared_ptr<PkbQueryHandler> pkbQH_ptr = make_shared<PkbQueryHandler>(pkb.get());

  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH_ptr);
  SpDriver spDriver;

  spDriver.parseSource("procedure Example {\n"
                       "  x = 2;\n" // 1
                       "  z = 3;\n" // 2
                       "  i = 5;\n" // 3
                       "  while (i!=0) {\n" // 4
                       "    x = x - 1;\n" // 5
                       "    if (x==1) then {\n" // 6
                       "      z = x + 1; }\n" // 7
                       "    else {\n"
                       "      y = z + x; }\n" // 8
                       "    z = z + x + i;\n" // 9
                       "    read x;\n" // 10
                       "    i = i - 1; }\n" // 11
                       "  z = x + 1; }" // 12
                       , pkbWriter.get());

  launchQuery2(qps.get(), "if ifs; stmt s;\n"
                          "Select ifs such that Follows(ifs, s)",
              unordered_set<string>{ "6" });

  // TODO(KwanHW): To verify results when PKB implementation is up
  // Tests disabled as it will result in failure for now
  string query;
  unordered_set<string> expectedRes;


//  query = "assign a; Select a such that Uses(a, \"x\")";
//  expectedRes = unordered_set<string>({"5", "7", "8", "9", "12"});
//  launchQuery2(qps.get(), query, expectedRes);
//
//  query = "assign a; Select a such that Modifies(a, \"x\")";
//  expectedRes = unordered_set<string>({"1", "5"});
//  launchQuery2(qps.get(), query, expectedRes);
}
