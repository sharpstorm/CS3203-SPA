#include "catch.hpp"
#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/query/PQLSelectContext.h"
#include "qps/errors/QPSParserError.h"

using std::make_unique;

TEST_CASE("Test PQL Select parsing") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("s")
      ->build();

  QueryTokenParseState state(&dummyStream);
  state.getQueryBuilder()->addVariable("s", PQL_SYN_TYPE_STMT);
  context.parse(&state);

  auto resultVar = state.getQueryBuilder()->build()->getResultVariable();
  REQUIRE(resultVar.name == "s");
  REQUIRE(resultVar.type == PQL_SYN_TYPE_STMT);
}

TEST_CASE("Test PQL Select unknown synonym") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("s")
      ->build();
  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}

TEST_CASE("Test PQL Select bad symbol") {
  PQLSelectContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->semicolon()
      ->build();
  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
