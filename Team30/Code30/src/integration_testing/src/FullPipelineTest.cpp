#include "catch.hpp"

#include <memory>
#include <unordered_set>
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
  vector<string>* result = nullptr;
  try {
    result = qps->evaluate(query);
  } catch (const QPSError& ex) {
    FAIL("Fail with error: " + ex.message);
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
                       "  x = 2;\n"
                       "  z = 3;\n"
                       "  i = 5;\n"
                       "  while (i!=0) {\n"
                       "    x = x - 1;\n"
                       "    if (x==1) then {\n"
                       "      z = x + 1; }\n"
                       "    else {\n"
                       "      y = z + x; }\n"
                       "    z = z + x + i;\n"
                       "    read x;\n"
                       "    i = i - 1; }\n"
                       "  z = x + 1; }", pkbWriter.get());

  launchQuery2(qps.get(), "if ifs; stmt s;\n"
                          "Select if such that Follows(ifs, s)",
              unordered_set<string>{ "6" });

}
