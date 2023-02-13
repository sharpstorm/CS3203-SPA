#include "catch.hpp"

#include <memory>
#include <unordered_set>
#include "qps/QPSFacade.h"
#include "pkb/writers/PkbWriter.h"
#include "qps/errors/QPSError.h"
#include "common/UtilityTypes.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

/*
 * procedure ANYA {
 *   x = 1;
 *   y = 2;
 *   z = 3;
 * }
 *
 */

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
  assertSetEquality(answer, resultSet);
}

TEST_CASE("Test QP Query Basic Follows") {
  auto pkb = make_unique<PKB>();
  auto pkbWriter = make_unique<PkbWriter>(pkb.get());
  shared_ptr<PkbQueryHandler> pkbQH_ptr = make_shared<PkbQueryHandler>(pkb.get());

  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH_ptr);

  pkbWriter->addProcedure("ANYA", 1, 5);
  pkbWriter->addStatement(1, StmtType::Assign);
  pkbWriter->addStatement(2, StmtType::Assign);
  pkbWriter->addStatement(3, StmtType::Assign);
  pkbWriter->addStatement(4, StmtType::Assign);
  pkbWriter->addStatement(5, StmtType::Assign);

  pkbWriter->addSymbol("x", EntityType::Variable);
  pkbWriter->addSymbol("y", EntityType::Variable);
  pkbWriter->addSymbol("z", EntityType::Variable);

  pkbWriter->addFollows(1, 2);
  pkbWriter->addFollows(2, 3);
  pkbWriter->addFollows(3, 4);
  pkbWriter->addFollows(4, 5);

  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Follows(s1, s2)",
              unordered_set<string>{ "1", "2", "3", "4" });
  launchQuery(qps.get(), "stmt s2; Select s2 such that Follows(1, s2)",
              unordered_set<string>{ "2" });
  launchQuery(qps.get(), "stmt s2; Select s2 such that Follows(s2, 4)",
              unordered_set<string>{ "3" });
  launchQuery(qps.get(), "assign s; Select s such that Follows(1, 2)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });
  launchQuery(qps.get(), "stmt s; Select s such that Follows(1, 3)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "assign a; stmt s1; Select s1 such that Follows(a, s1)",
              unordered_set<string>{ "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign a; read r; Select r such that Follows(a, r)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(1, s2)",
              unordered_set<string>{ "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(2, s2)",
              unordered_set<string>{ "3", "4", "5" });

  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(s2, 4)",
              unordered_set<string>{ "1", "2", "3" });
  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows*(s2, 4)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });

  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows(1, s2)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows(5, s2)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "assign s1, s2; Select s1",
              unordered_set<string>{ "1", "2", "3", "4", "5" });

  launchQuery(qps.get(), "variable v; Select v",
              unordered_set<string>{ "x", "y", "z" });

  launchQuery(qps.get(), "variable v; Select v such that Uses(1, v)", unordered_set<string>{});

}
