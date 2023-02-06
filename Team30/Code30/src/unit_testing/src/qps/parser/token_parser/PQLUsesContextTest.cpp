#include "catch.hpp"

#include "qps/parser/token_parser/context/such_that_clause/PQLUsesClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/UsesClause.h"

void testUsesParsing(vector<PQLToken> inputs) {
  PQLUsesClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  state.getQueryBuilder()->addVariable("v", PQL_VAR_TYPE_VARIABLE);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<UsesClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Uses parsing 2 Constants") {
  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "7"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_INTEGER, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "main"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_INTEGER, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Uses 1 Constant Left") {
  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "main"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "v"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "v"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Uses 1 Constant Right") {
  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Uses 0 Constant") {
  testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "v"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Uses unknown ref") {
  REQUIRE_THROWS_AS(testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Uses Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Uses bad syntax") {
  REQUIRE_THROWS_AS(testUsesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

