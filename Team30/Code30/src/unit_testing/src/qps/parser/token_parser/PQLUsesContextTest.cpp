#include "catch.hpp"
#include <memory>
#include <unordered_map>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLUsesClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/UsesClause.h"
#include "qps/parser/builder/QueryBuilderError.h"

using std::make_unique, std::unordered_map;

void testUsesParsing(vector<PQLToken> inputs,
                     unordered_map<string, PQLSynonymType> synonyms) {
  PQLUsesClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);

  for (auto it : synonyms) {
    state.getQueryBuilder()->addVariable(it.first, it.second);
  }
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<UsesClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

void testUsesParsing(vector<PQLToken> inputs) {
  testUsesParsing(inputs, unordered_map<string, PQLSynonymType>{
      {"a", PQL_VAR_TYPE_ASSIGN},
      {"p", PQL_VAR_TYPE_PROCEDURE},
      {"v", PQL_VAR_TYPE_VARIABLE}
  });
}

TEST_CASE("Test PQL Uses parsing 2 Constants") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(7)
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 1 Constant Left") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 1 Constant Right") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 0 Constant") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses unknown ref") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("a")
                              ->comma()
                              ->synonym("w")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("w")
                              ->comma()
                              ->synonym("v")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Uses Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("a")
                              ->comma()
                              ->integer(1)
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Uses wildcard not allowed on left") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->wildcard()
                              ->comma()
                              ->ident("x")
                              ->closeBracket()
                              ->build()
      ), QueryBuilderError
  );
}

TEST_CASE("Test PQL Uses bad syntax") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->ident("x")
                          ->closeBracket()
                          ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->comma()
                          ->ident("x")
                          ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Uses valid synonym types") {
  auto validTypes = vector<PQLSynonymType>{
      PQL_VAR_TYPE_ASSIGN,
      PQL_VAR_TYPE_PRINT,
      PQL_VAR_TYPE_IF,
      PQL_VAR_TYPE_WHILE,
      PQL_VAR_TYPE_CALL,
      PQL_VAR_TYPE_PROCEDURE,
  };

  for (PQLSynonymType type : validTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"v", PQL_VAR_TYPE_VARIABLE}
    };
    testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                            ->openBracket()
                            ->synonym("s1")
                            ->comma()
                            ->synonym("v")
                            ->closeBracket()
                            ->build(),
                        synonymMap
    );
  }
}

TEST_CASE("Test PQL Uses invalid left synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_VAR_TYPE_VARIABLE,
      PQL_VAR_TYPE_CONSTANT,
      PQL_VAR_TYPE_STMT,
      PQL_VAR_TYPE_READ
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"v", PQL_VAR_TYPE_VARIABLE}
    };

    REQUIRE_THROWS_AS(
        testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                                ->openBracket()
                                ->synonym("s1")
                                ->comma()
                                ->synonym("v")
                                ->closeBracket()
                                ->build(),
                            synonymMap
        ), QueryBuilderError
    );
  }
}

TEST_CASE("Test PQL Uses invalid right synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_VAR_TYPE_CONSTANT,
      PQL_VAR_TYPE_PROCEDURE,
      PQL_VAR_TYPE_STMT,
      PQL_VAR_TYPE_READ,
      PQL_VAR_TYPE_PRINT,
      PQL_VAR_TYPE_CALL,
      PQL_VAR_TYPE_WHILE,
      PQL_VAR_TYPE_IF,
      PQL_VAR_TYPE_ASSIGN
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"v", type}
    };

    REQUIRE_THROWS_AS(
        testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                                ->openBracket()
                                ->integer(1)
                                ->comma()
                                ->synonym("v")
                                ->closeBracket()
                                ->build(),
                            synonymMap
        ), QueryBuilderError
    );
  }
}
