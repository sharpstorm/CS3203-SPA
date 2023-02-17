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

  auto resultVar = state.getQueryBuilder()->build()->getResultVariable();
  REQUIRE(resultVar.getName() == "s");
  REQUIRE(resultVar.getType() == PQL_SYN_TYPE_STMT);
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
