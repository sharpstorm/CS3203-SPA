#include "catch.hpp"

#include <memory>
#include <string>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/declarations/PQLStmtContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/parser/lexer/QueryLexerTokenTable.h"

using std::make_unique, std::string;

TEST_CASE("Test PQL Declaration parsing") {
  PQLStmtContext context;

  vector<string> vars = vector<string>{"s", "s0", "a0s", "a00sdf00", "abcdefd",
                                       "asDFG", "ASD", "A0Sw9"};

  for (string v : vars) {
    auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
        ->synonym(v)
        ->semicolon()
        ->build();

    QueryTokenParseState state(&dummyStream);
    context.parse(&state);
    REQUIRE(state.getQueryBuilder()->hasVariable(v));
  }
}

TEST_CASE("Test PQL Declaration Keyword Name") {
  PQLStmtContext context;

  QueryLexerTokenTable tokenTable;
  for (auto it : tokenTable.keywordMap) {
    auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
        ->addToken(it.second, it.first)
        ->semicolon()
        ->build();
    QueryTokenParseState state(&dummyStream);
    context.parse(&state);
    REQUIRE(state.getQueryBuilder()->hasVariable(it.first));
  }
}

TEST_CASE("Test PQL Declaration Bad Synonym Name") {
  PQLStmtContext context;

  vector<string> vars = vector<string>{"0as", "1234"};
  for (string v : vars) {
    auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
        ->synonym(v)
        ->semicolon()
        ->build();

    QueryTokenParseState state(&dummyStream);
    context.parse(&state);
    REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
  }
}

TEST_CASE("Test PQL Declaration No semicolon") {
  PQLStmtContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->synonym("s")
      ->build();

  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
