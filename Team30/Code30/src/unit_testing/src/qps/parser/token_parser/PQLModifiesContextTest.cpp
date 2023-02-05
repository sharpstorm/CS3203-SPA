#include "catch.hpp"

#include "qps/parser/token_parser/context/such_that_clause/PQLModifiesClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/ModifiesClause.h"

void testModifiesParsing(vector<PQLToken> inputs) {
  PQLModifiesClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ModifiesClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Modifies parsing 2 Constants") {
  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "7"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_INTEGER, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testModifiesParsing(vector<PQLToken>{
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

TEST_CASE("Test PQL Modifies 1 Constant Left") {
  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "main"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "s"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Modifies 1 Constant Right") {
  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Modifies 0 Constant") {
  testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Modifies unknown ref") {
  REQUIRE_THROWS_AS(testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "w"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Modifies Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Modifies bad syntax") {
  REQUIRE_THROWS_AS(testModifiesParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

