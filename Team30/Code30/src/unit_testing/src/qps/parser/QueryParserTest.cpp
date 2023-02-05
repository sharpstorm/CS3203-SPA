#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "catch.hpp"
#include "qps/parser/lexer/QueryLexer.h"
#include "qps/common/PQLQuery.h"
#include "qps/errors/QPSError.h"
#include "qps/parser/token_parser/QueryTokenParser.h"
#include "qps/clauses/FollowsClause.h"
#include "qps/parser/QueryParser.h"

using std::vector, std::string, std::cout, std::unique_ptr, std::shared_ptr;

unique_ptr<PQLQuery> testPQLParsing(string testCase) {
  unique_ptr<PQLQuery> result = QueryParser().parseQuery(&testCase);

  try {
    result = parser.build();
  } catch (const QPSError& err) {
    FAIL("FAILED with error: " + err.message);
  } catch (...) {
    FAIL("Failed with unknown exception");
  }

  return result;
}

void requireSynonyms(shared_ptr<PQLQuery> result, vector<PQLQueryVariable> expectedVariables) {
  REQUIRE(result->getVariableCount() == expectedVariables.size());
  for (int i = 0; i < expectedVariables.size(); i++) {
    auto expectedVar = expectedVariables.at(i);
    auto var = result->getVariable(expectedVar.name);
    REQUIRE(var != nullptr);
    REQUIRE(var->name == expectedVar.name);
    REQUIRE(var->type == expectedVar.type);
  }
}

TEST_CASE("Test Unconditional Select") {
  auto result = testPQLParsing("stmt s; Select s");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s"}
  });
  REQUIRE(resultShared->getResultVariable().name == "s");
  REQUIRE(resultShared->getResultVariable().type == PQL_VAR_TYPE_STMT);
  REQUIRE(resultShared->getEvaluatables().size() == 0);
}

TEST_CASE("Test Conditional Select - 2 Constants") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(1, 2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s"}
  });
  REQUIRE(resultShared->getResultVariable().name == "s");
  REQUIRE(resultShared->getResultVariable().type == PQL_VAR_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test Conditional Select - 1 Constant") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(s, 2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s"}
  });
  REQUIRE(resultShared->getResultVariable().name == "s");
  REQUIRE(resultShared->getResultVariable().type == PQL_VAR_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);

  result = testPQLParsing("stmt s; Select s such that Follows(3, s)");
  resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s"}
  });
  REQUIRE(resultShared->getResultVariable().name == "s");
  REQUIRE(resultShared->getResultVariable().type == PQL_VAR_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test Conditional Select - 0 Constants") {
  auto result = testPQLParsing("stmt s1, s2; Select s1 such that Follows(s1, s2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s1"},
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "s2"}
  });
  REQUIRE(resultShared->getResultVariable().name == "s1");
  REQUIRE(resultShared->getResultVariable().type == PQL_VAR_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}
