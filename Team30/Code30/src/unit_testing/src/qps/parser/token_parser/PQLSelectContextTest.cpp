#include "catch.hpp"
#include <memory>

#include "PQLContextTestUtils.h"
#include "../../util/PQLTestTokenSequenceBuilder.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/parsers/select_parser/PQLSelectParser.h"

using std::make_unique;

void testSynAttribute(AttributedSynonym syn,
                      vector<PQLToken> tokenSeq) {
  QueryBuilder builder;
  builder.addSynonym(syn.getName(), syn.getType());
  builder.finalizeSynonymTable();

  testParsing<PQLSelectParser>(tokenSeq, &builder);

  auto query = builder.build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 1);
  REQUIRE(resultVar->at(0).getName() == syn.getName());
  REQUIRE(resultVar->at(0).getType() == syn.getType());
  REQUIRE(resultVar->at(0).getAttribute() == syn.getAttribute());
}

TEST_CASE("Test PQL Select parsing") {
  QueryBuilder builder;
  builder.addSynonym("s", PQL_SYN_TYPE_STMT);
  builder.finalizeSynonymTable();

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
  builder.finalizeSynonymTable();

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
          ->addToken(PQL_TOKEN_BOOLEAN)
          ->build());

  auto query = builder->build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 0);
}

TEST_CASE("Test PQL Select Declared BOOLEAN Name parsing") {
  QueryBuilder qb;
  qb.addSynonym("BOOLEAN", PQL_SYN_TYPE_ASSIGN);
  qb.finalizeSynonymTable();
  testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("BOOLEAN")
          ->build(), &qb);

  auto query = qb.build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 1);
  REQUIRE(resultVar->at(0).getName() == "BOOLEAN");
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

TEST_CASE("Test PQL Select attribute parsing") {
  auto querySyn = make_unique<PQLQuerySynonym>(PQL_SYN_TYPE_STMT, "s1");
  auto ptr = querySyn.get();
  PQLQuerySynonymProxy proxy(&ptr);
  AttributedSynonym syn = AttributedSynonym(proxy, STMT_NUM);

  auto tokenSeq = make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("s1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_STMT)
          ->addToken(PQL_TOKEN_NUMBER_SIGN)
          ->addToken(PQL_TOKEN_EQUALS)
          ->integer(1)->build();
  testSynAttribute(syn, tokenSeq);

  querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym(PQL_SYN_TYPE_VARIABLE, "v1"));
  ptr = querySyn.get();
  proxy = PQLQuerySynonymProxy(&ptr);
  syn = AttributedSynonym(proxy, VAR_NAME);
  tokenSeq = make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("v1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_VAR_NAME)
          ->addToken(PQL_TOKEN_EQUALS)
          ->ident("x")->build();

  testSynAttribute(syn, tokenSeq);


  querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym(PQL_SYN_TYPE_PROCEDURE, "p1"));
  ptr = querySyn.get();
  proxy = PQLQuerySynonymProxy(&ptr);
  syn = AttributedSynonym(proxy, PROC_NAME);
  tokenSeq = make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("p1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_PROC_NAME)
          ->addToken(PQL_TOKEN_EQUALS)
          ->ident("x")->build();

  testSynAttribute(syn, tokenSeq);

  querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym(PQL_SYN_TYPE_CONSTANT, "c1"));
  ptr = querySyn.get();
  proxy = PQLQuerySynonymProxy(&ptr);
  syn = AttributedSynonym(proxy, CONST_VALUE);
  tokenSeq = make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->synonym("c1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_VALUE)
          ->addToken(PQL_TOKEN_EQUALS)
          ->integer(8)->build();

  testSynAttribute(syn, tokenSeq);
}

TEST_CASE("Test PQL Select attribute in tuple parsing") {
  QueryBuilder builder;
  builder.addSynonym("s1", PQL_SYN_TYPE_STMT);
  builder.addSynonym("v1", PQL_SYN_TYPE_VARIABLE);
  builder.addSynonym("p1", PQL_SYN_TYPE_PROCEDURE);
  builder.addSynonym("c1", PQL_SYN_TYPE_CONSTANT);
  builder.finalizeSynonymTable();

  // Select <s1.stmt#, v1.varName, p1.procName, c1.value>
  testParsing<PQLSelectParser>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_SELECT)
          ->addToken(PQL_TOKEN_TUPLE_OPEN)
          ->synonym("s1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_STMT)
          ->addToken(PQL_TOKEN_NUMBER_SIGN)
          ->comma()
          ->synonym("v1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_VAR_NAME)
          ->comma()
          ->synonym("p1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_PROC_NAME)
          ->comma()
          ->synonym("c1")
          ->addToken(PQL_TOKEN_PERIOD)
          ->addToken(PQL_TOKEN_VALUE)
          ->addToken(PQL_TOKEN_TUPLE_CLOSE)
          ->build(), &builder);

  auto query = builder.build();
  auto resultVar = query->getResultVariables();
  REQUIRE(resultVar->size() == 4);
  REQUIRE(resultVar->at(0).getName() == "s1");
  REQUIRE(resultVar->at(0).getType() == PQL_SYN_TYPE_STMT);
  REQUIRE(resultVar->at(0).getAttribute() == STMT_NUM);
  REQUIRE(resultVar->at(1).getName() == "v1");
  REQUIRE(resultVar->at(1).getType() == PQL_SYN_TYPE_VARIABLE);
  REQUIRE(resultVar->at(1).getAttribute() == VAR_NAME);
  REQUIRE(resultVar->at(2).getName() == "p1");
  REQUIRE(resultVar->at(2).getType() == PQL_SYN_TYPE_PROCEDURE);
  REQUIRE(resultVar->at(2).getAttribute() == PROC_NAME);
  REQUIRE(resultVar->at(3).getName() == "c1");
  REQUIRE(resultVar->at(3).getType() == PQL_SYN_TYPE_CONSTANT);
  REQUIRE(resultVar->at(3).getAttribute() == CONST_VALUE);
}
