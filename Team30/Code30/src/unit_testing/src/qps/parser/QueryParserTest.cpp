#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "catch.hpp"
#include "qps/parser/lexer/QueryLexer.h"
#include "qps/common/PQLQuery.h"
#include "qps/errors/QPSError.h"
#include "qps/parser/token_parser/QueryTokenParser.h"
#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/parser/QueryParser.h"
#include "qps/clauses/pattern/AssignPatternClause.h"

using std::vector, std::string, std::cout, std::unique_ptr, std::shared_ptr;

unique_ptr<PQLQuery> testPQLParsing(string testCase) {
  try {
    return QueryParser().parseQuery(&testCase);
  } catch (const QPSError& err) {
    WARN(err.what());
    FAIL("FAILED with error: ");
  } catch (...) {
    FAIL("Failed with unknown exception");
  }

  FAIL("Failed with unknown exception");
  return nullptr;
}

void requireSynonyms(shared_ptr<PQLQuery> result, vector<PQLQuerySynonym> expectedVariables) {
  REQUIRE(result->getVariableCount() == expectedVariables.size());
  for (int i = 0; i < expectedVariables.size(); i++) {
    auto expectedVar = expectedVariables.at(i);
    auto var = result->getVariable(expectedVar.getName());
    REQUIRE(var != nullptr);
    REQUIRE(*var == expectedVar);
  }
}

TEST_CASE("Test Unconditional Select") {
  auto result = testPQLParsing("stmt s; Select s");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "s");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_STMT);
  REQUIRE(resultShared->getEvaluatables().size() == 0);
}

TEST_CASE("Test Conditional Select - 2 Constants") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(1, 2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "s");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test Conditional Select - 1 Constant") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(s, 2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "s");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);

  result = testPQLParsing("stmt s; Select s such that Follows(3, s)");
  resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "s");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test Conditional Select - 0 Constants") {
  auto result = testPQLParsing("stmt s1, s2; Select s1 such that Follows(s1, s2)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s1"),
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s2")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "s1");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_STMT);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  FollowsClause* fc = dynamic_cast<FollowsClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test Pattern Select - Constant LHS") {
  auto result = testPQLParsing("assign a; Select a pattern a(\"x\", \"y\")");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "a");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_ASSIGN);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  AssignPatternClause* pc = dynamic_cast<AssignPatternClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(pc != nullptr);
}

TEST_CASE("Test Pattern Select - Wildcard LHS, Partial Match") {
  auto result = testPQLParsing("assign a; Select a pattern a(_, _\"y\"_)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a")
  });
  REQUIRE(resultShared->getResultVariable().getName() == "a");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_ASSIGN);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  AssignPatternClause* pc = dynamic_cast<AssignPatternClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(pc != nullptr);
}

TEST_CASE("Test Pattern Select - Variable, Wildcard") {
  auto result = testPQLParsing("assign a; variable v; Select v pattern a(v, _)");
  shared_ptr<PQLQuery> resultShared = shared_ptr<PQLQuery>(std::move(result));

  requireSynonyms(resultShared, vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a"),
      PQLQuerySynonym(PQL_SYN_TYPE_VARIABLE, "v"),
  });
  REQUIRE(resultShared->getResultVariable().getName() == "v");
  REQUIRE(resultShared->getResultVariable().getType() == PQL_SYN_TYPE_VARIABLE);

  REQUIRE(resultShared->getEvaluatables().size() == 1);
  AssignPatternClause* pc = dynamic_cast<AssignPatternClause*>(resultShared->getEvaluatables().at(0).get());
  REQUIRE(pc != nullptr);
}
