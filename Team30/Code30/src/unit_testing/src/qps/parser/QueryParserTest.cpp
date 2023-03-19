#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "catch.hpp"
#include "qps/common/PQLQuery.h"
#include "qps/errors/QPSError.h"
#include "qps/parser/token_parser/QueryTokenParser.h"
#include "qps/parser/QueryParser.h"
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "../SourceParserStub.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/errors/QPSLexerError.h"
#include "qps/clauses/such_that/FollowsClauses.h"
#include "qps/clauses/such_that/ModifiesClause.h"

using std::vector, std::string, std::cout, std::unique_ptr, std::shared_ptr;

unique_ptr<PQLQuery> testPQLParsing(string testCase) {
  SourceParserStub exprParser;
  try {
    return QueryParser(&exprParser).parseQuery(&testCase);
  } catch (const QPSError& err) {
    WARN(err.what());
    FAIL("FAILED with error: ");
  } catch (...) {
    FAIL("Failed with unknown exception");
  }

  FAIL("Failed with unknown exception");
  return nullptr;
}

void expectPQLSyntaxError(string testCase) {
  SourceParserStub exprParser;
  try {
    QueryParser(&exprParser).parseQuery(&testCase);
    FAIL("No Syntax Exception Thrown");
  } catch (const QPSParserSyntaxError& err) {
  } catch (const QPSLexerError& err) {
  } catch (const QPSParserSemanticError& err) {
    FAIL("Semantic error wrongly Thrown");
  } catch (...) {
    FAIL("Failed with unknown exception");
  }
}

void requireSynonyms(PQLQuery* result, vector<PQLQuerySynonym> expectedVariables) {
  REQUIRE(result->getVariableCount() == expectedVariables.size());
  for (int i = 0; i < expectedVariables.size(); i++) {
    auto expectedVar = expectedVariables.at(i);
    auto var = result->getVariable(expectedVar.getName());
    REQUIRE(var != nullptr);
    REQUIRE(**var == expectedVar);
  }
}

void requireResultSynonyms(PQLQuery* result, unordered_map<PQLSynonymName, PQLSynonymType> expectedVariables) {
  REQUIRE(result->getResultVariables()->size() == expectedVariables.size());
  unordered_set<PQLSynonymName> seen;

  for (auto syn : *result->getResultVariables()) {
    auto it = expectedVariables.find(syn.getName());
    REQUIRE(it != expectedVariables.end());
    REQUIRE(it->second == syn.getType());
    seen.insert(syn.getName());
  }

  REQUIRE(seen.size() == expectedVariables.size());
}

template <class T>
void requireClauseExists(PQLQuery* result) {
  REQUIRE(result->getEvaluatables().size() == 1);
  T* casted = dynamic_cast<T*>(result->getEvaluatables().at(0).get());
  REQUIRE(casted != nullptr);
}

void requireNoClause(PQLQuery* result) {
  REQUIRE(result->getEvaluatables().size() == 0);
}

TEST_CASE("Test Unconditional Select") {
  auto result = testPQLParsing("stmt s; Select s");
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireNoClause(result.get());
}

TEST_CASE("Test Bad Synonym Name") {
  expectPQLSyntaxError("stmt 0asd; Select 0asd");
  expectPQLSyntaxError("stmt 1234; Select 1234");
}

TEST_CASE("Test Conditional Select - 2 Constants") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(1, 2)");
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireClauseExists<FollowsClause>(result.get());
}

TEST_CASE("Test Conditional Select - 1 Constant") {
  auto result = testPQLParsing("stmt s; Select s such that Follows(s, 2)");
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireClauseExists<FollowsClause>(result.get());

  result = testPQLParsing("stmt s; Select s such that Follows(3, s)");
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s")
  });
  requireResultSynonyms(result.get(), {
      {"s", PQL_SYN_TYPE_STMT}
  });
  requireClauseExists<FollowsClause>(result.get());
}

TEST_CASE("Test Conditional Select - 0 Constants") {
  auto expectedVariables = vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s1"),
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "s2")
  };
  auto expectedResultVars = unordered_map<PQLSynonymName, PQLSynonymType>({
      {"s1", PQL_SYN_TYPE_STMT}
  });

  auto result = testPQLParsing("stmt s1, s2; Select s1 such that Follows(s1, s2)");

  requireResultSynonyms(result.get(), expectedResultVars);
  requireSynonyms(result.get(), expectedVariables);
  requireClauseExists<FollowsClause>(result.get());
}

TEST_CASE("Test Pattern Select - Constant LHS") {
  auto result = testPQLParsing("assign a; Select a pattern a(\"x\", \"y\")");
  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a")
  });
  requireResultSynonyms(result.get(), {
      {"a", PQL_SYN_TYPE_ASSIGN}
  });
  requireClauseExists<AssignPatternClause>(result.get());
}

TEST_CASE("Test Pattern Select - Wildcard LHS, Partial Match") {
  auto result = testPQLParsing("assign a; Select a pattern a(_, _\"y\"_)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a")
  });
  requireResultSynonyms(result.get(), {
      {"a", PQL_SYN_TYPE_ASSIGN}
  });
  requireClauseExists<AssignPatternClause>(result.get());
}

TEST_CASE("Test Pattern Select - Variable, Wildcard") {
  auto result = testPQLParsing("assign a; variable v; Select v pattern a(v, _)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a"),
      PQLQuerySynonym(PQL_SYN_TYPE_VARIABLE, "v"),
  });
  requireResultSynonyms(result.get(), {
      {"v", PQL_SYN_TYPE_VARIABLE}
  });
  requireClauseExists<AssignPatternClause>(result.get());
}

TEST_CASE("Test ModifiesP - Constant, Constant") {
  auto result = testPQLParsing("print p; Select p such that Modifies(\"Main\", \"a1\")");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"),
  });
  requireResultSynonyms(result.get(), {
      {"p", PQL_SYN_TYPE_PRINT}
  });

  requireClauseExists<ModifiesClause>(result.get());
}

TEST_CASE("Test Next - Constant, Constant") {
  auto result = testPQLParsing("print p; Select p such that Next(1, 2)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"),
  });

  requireResultSynonyms(result.get(), {
      {"p", PQL_SYN_TYPE_PRINT}
  });

  requireClauseExists<NextClause>(result.get());
}

TEST_CASE("Test Next* - Constant, Constant") {
  auto result = testPQLParsing("print p; Select p such that Next*(1, 2)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"),
  });

  requireResultSynonyms(result.get(), {
      {"p", PQL_SYN_TYPE_PRINT}
  });

  requireClauseExists<NextTClause>(result.get());
}

TEST_CASE("Test Affects - Constant, Constant") {
  auto result = testPQLParsing("print p; Select p such that Affects(1, 2)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"),
  });

  requireResultSynonyms(result.get(), {
      {"p", PQL_SYN_TYPE_PRINT}
  });

  requireClauseExists<AffectsClause>(result.get());
}

TEST_CASE("Test Affects* - Constant, Constant") {
  auto result = testPQLParsing("print p; Select p such that Affects*(1, 2)");

  requireSynonyms(result.get(), vector<PQLQuerySynonym>{
      PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"),
  });

  requireResultSynonyms(result.get(), {
      {"p", PQL_SYN_TYPE_PRINT}
  });

  requireClauseExists<AffectsTClause>(result.get());
}
