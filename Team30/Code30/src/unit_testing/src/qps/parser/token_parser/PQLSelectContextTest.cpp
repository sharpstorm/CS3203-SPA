#include "catch.hpp"
#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/query/PQLSelectContext.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique;

TEST_CASE("Test PQL Select parsing") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("s")
      ->build();

  QueryTokenParseState state(&dummyStream);
  state.getQueryBuilder()->addSynonym("s", PQL_SYN_TYPE_STMT);
  context.parse(&state);

  auto query = state.getQueryBuilder()->build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 1);
  REQUIRE(resultVar->at(0).getName() == "s");
  REQUIRE(resultVar->at(0).getType() == PQL_SYN_TYPE_STMT);
}

TEST_CASE("Test PQL Select Tuple parsing") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->addToken(PQL_TOKEN_TUPLE_OPEN)
      ->synonym("s1")
      ->comma()
      ->synonym("s2")
      ->comma()
      ->synonym("s3")
      ->addToken(PQL_TOKEN_TUPLE_CLOSE)
      ->build();

  QueryTokenParseState state(&dummyStream);
  state.getQueryBuilder()->addSynonym("s1", PQL_SYN_TYPE_STMT);
  state.getQueryBuilder()->addSynonym("s2", PQL_SYN_TYPE_STMT);
  state.getQueryBuilder()->addSynonym("s3", PQL_SYN_TYPE_STMT);
  context.parse(&state);

  auto query = state.getQueryBuilder()->build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 3);
  REQUIRE(resultVar->at(0).getName() == "s1");
  REQUIRE(resultVar->at(0).getType() == PQL_SYN_TYPE_STMT);
  REQUIRE(resultVar->at(1).getName() == "s2");
  REQUIRE(resultVar->at(1).getType() == PQL_SYN_TYPE_STMT);
  REQUIRE(resultVar->at(2).getName() == "s3");
  REQUIRE(resultVar->at(2).getType() == PQL_SYN_TYPE_STMT);
}

TEST_CASE("Test PQL Select Boolean parsing") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("BOOLEAN")
      ->build();

  QueryTokenParseState state(&dummyStream);
  context.parse(&state);

  auto query = state.getQueryBuilder()->build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 0);
}

TEST_CASE("Test PQL Select unknown synonym") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("s")
      ->build();
  QueryTokenParseState state(&dummyStream);
  context.parse(&state);
  REQUIRE_THROWS_AS(state.getQueryBuilder()->build(), QPSParserSemanticError);
}

TEST_CASE("Test PQL Select bad symbol") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->semicolon()
      ->build();
  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserSyntaxError);
}
