#include "catch.hpp"

#include "qps/parser/token_parser/context/such_that_clause/PQLFollowsClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/FollowsClause.h"

void testFollowsParsing(vector<PQLToken> inputs) {
  PQLFollowsClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<FollowsClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Follows parsing 2 Constants") {
  testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Follows 1 Constant Left") {
  testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Follows 1 Constant Right") {
  testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Follows 0 Constant") {
  testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Follows unknown ref") {
  REQUIRE_THROWS_AS(testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Follows Entity ref not allowed") {
  REQUIRE_THROWS_AS(testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);

  REQUIRE_THROWS_AS(testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Follows bad syntax") {
  REQUIRE_THROWS_AS(testFollowsParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

