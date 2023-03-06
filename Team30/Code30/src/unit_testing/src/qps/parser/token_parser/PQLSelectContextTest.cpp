#include "catch.hpp"
#include <memory>

#include "PQLContextTestUtils.h"
#include "../../util/PQLTestTokenSequenceBuilder.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/parsers/select_parser/PQLSelectParser.h"

using std::make_unique;

TEST_CASE("Test PQL Select parsing") {
  QueryBuilder builder;
  builder.addSynonym("s", PQL_SYN_TYPE_STMT);

  testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("s")
          ->build(), &builder);

  auto query = builder.build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 1);
  REQUIRE(resultVar->at(0).getName() == "s");
  REQUIRE(resultVar->at(0).getType() == PQL_SYN_TYPE_STMT);
}

TEST_CASE("Test PQL Select Tuple parsing") {
  QueryBuilder builder;
  builder.addSynonym("s1", PQL_SYN_TYPE_STMT);
  builder.addSynonym("s2", PQL_SYN_TYPE_STMT);
  builder.addSynonym("s3", PQL_SYN_TYPE_STMT);

  testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->addToken(PQL_TOKEN_TUPLE_OPEN)
          ->synonym("s1")
          ->comma()
          ->synonym("s2")
          ->comma()
          ->synonym("s3")
          ->addToken(PQL_TOKEN_TUPLE_CLOSE)
          ->build(), &builder);

  auto query = builder.build();
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
  auto builder = testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("BOOLEAN")
          ->build());

  auto query = builder->build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 0);
}

TEST_CASE("Test PQL Select unknown synonym") {
  auto builder = testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("s")
          ->build());
  REQUIRE_THROWS_AS(builder->build(), QPSParserSemanticError);
}

TEST_CASE("Test PQL Select bad symbol") {
  REQUIRE_THROWS_AS(testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->semicolon()
          ->build()), QPSParserSyntaxError);
}
