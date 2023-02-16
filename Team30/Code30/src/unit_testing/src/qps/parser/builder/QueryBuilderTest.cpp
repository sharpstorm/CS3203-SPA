#include "catch.hpp"
#include "qps/parser/builder/QueryBuilder.h"
#include "qps/clauses/FollowsClause.h"
#include "qps/clauses/ParentClause.h"
#include "qps/clauses/ModifiesClause.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique, std::move;

TEST_CASE("Test QueryBuilder Success") {
  QueryBuilder qb;
  qb.addSynonym("a", PQL_SYN_TYPE_STMT);
  qb.addSynonym("b", PQL_SYN_TYPE_PROCEDURE);
  qb.addSynonym("c", PQL_SYN_TYPE_CONSTANT);
  qb.addSynonym("d", PQL_SYN_TYPE_VARIABLE);
  qb.addSynonym("e", PQL_SYN_TYPE_ASSIGN);
  qb.addSynonym("f", PQL_SYN_TYPE_WHILE);
  qb.addSynonym("g", PQL_SYN_TYPE_CALL);
  qb.addSynonym("h", PQL_SYN_TYPE_PRINT);
  qb.addSynonym("i", PQL_SYN_TYPE_READ);
  qb.addSynonym("j", PQL_SYN_TYPE_IF);

  qb.addSuchThat(make_unique<FollowsClause>(ClauseArgument(1), ClauseArgument(2)));
  qb.addSuchThat(make_unique<ParentClause>(ClauseArgument(
      PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a")), ClauseArgument(2)));

  REQUIRE(qb.hasSynonym("a"));
  REQUIRE(qb.hasSynonym("b"));
  REQUIRE(qb.hasSynonym("c"));
  REQUIRE(qb.hasSynonym("d"));
  REQUIRE(qb.hasSynonym("e"));
  REQUIRE(qb.hasSynonym("f"));
  REQUIRE(qb.hasSynonym("g"));
  REQUIRE(qb.hasSynonym("h"));
  REQUIRE(qb.hasSynonym("i"));
  REQUIRE(qb.hasSynonym("j"));

  REQUIRE(*qb.accessSynonym("a") == PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a"));
  REQUIRE(*qb.accessSynonym("i") == PQLQuerySynonym(PQL_SYN_TYPE_READ, "i"));

  auto result = qb.build();

  REQUIRE(result->getVariableCount() == 10);
  REQUIRE(result->getEvaluatables().size() == 2);
  REQUIRE(*result->getVariable("a") == PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a"));
  REQUIRE(*result->getVariable("h") == PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "h"));

  auto fc = dynamic_cast<FollowsClause*>(result->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
  auto pc = dynamic_cast<ParentClause*>(result->getEvaluatables().at(1).get());
  REQUIRE(pc != nullptr);
}

TEST_CASE("Test QueryBuilder Duplicate Variable") {
  QueryBuilder qb;
  qb.addSynonym("a", PQL_SYN_TYPE_STMT);
  qb.addSynonym("a", PQL_SYN_TYPE_PROCEDURE);
  REQUIRE_THROWS_AS(qb.build(), QPSParserSemanticError);
}

TEST_CASE("Test QueryBuilder Invalid Clause - Follows") {
  QueryBuilder qb;
  qb.addSynonym("a", PQL_SYN_TYPE_CONSTANT);
  qb.addSuchThat(make_unique<FollowsClause>(
      ClauseArgument(PQLQuerySynonym(PQL_SYN_TYPE_CONSTANT, "a")),
      ClauseArgument(2)));
  REQUIRE_THROWS_AS(qb.build(), QPSParserSemanticError);

  QueryBuilder qb2;
  qb2.addSynonym("a", PQL_SYN_TYPE_CONSTANT);
  qb2.addSuchThat(make_unique<FollowsClause>(
      ClauseArgument(2),
      ClauseArgument(PQLQuerySynonym(PQL_SYN_TYPE_CONSTANT, "a"))
  ));
  REQUIRE_THROWS_AS(qb2.build(), QPSParserSemanticError);
}

TEST_CASE("Test QueryBuilder Invalid Clause - Modifies") {
  QueryBuilder qb;
  qb.addSynonym("a", PQL_SYN_TYPE_CONSTANT);
  qb.addSuchThat(make_unique<ModifiesClause>(
      ClauseArgument(PQLQuerySynonym(PQL_SYN_TYPE_CONSTANT, "a")),
      ClauseArgument("a")));
  REQUIRE_THROWS_AS(qb.build(), QPSParserSemanticError);

  QueryBuilder qb2;
  qb2.addSuchThat(make_unique<ModifiesClause>(
      ClauseArgument(CLAUSE_ARG_WILDCARD),
      ClauseArgument("a")));
  REQUIRE_THROWS_AS(qb2.build(), QPSParserSemanticError);

  QueryBuilder qb3;
  qb3.addSynonym("a", PQL_SYN_TYPE_CONSTANT);
  qb3.addSuchThat(make_unique<ModifiesClause>(
      ClauseArgument("a"),
      ClauseArgument(PQLQuerySynonym(PQL_SYN_TYPE_CONSTANT, "a"))));
  REQUIRE_THROWS_AS(qb3.build(), QPSParserSemanticError);
}
