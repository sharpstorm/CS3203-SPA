#include "catch.hpp"

#include "qps/parser/token_parser/context/pattern_clause/PQLAssignPatternClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/AssignPatternClause.h"

void testAssignPatternParsing(vector<PQLToken> inputs) {
  PQLAssignPatternClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  state.getQueryBuilder()->addVariable("a", PQL_VAR_TYPE_ASSIGN);
  state.getQueryBuilder()->addVariable("x", PQL_VAR_TYPE_VARIABLE);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  state.getQueryBuilder()->addVariable("b", PQL_VAR_TYPE_CONSTANT);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<AssignPatternClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Assign Pattern parsing Full Match") {
  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Assign Pattern parsing Partial Match") {
  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Assign Pattern parsing Wildcard") {
  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });

  testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}

TEST_CASE("Test PQL Assign Pattern invalid ref") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "d"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "z"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "s"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "s"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "x"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_VARIABLE, "b"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

TEST_CASE("Test PQL Assign Pattern bad syntax") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_VARIABLE, "a"},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_VARIABLE, "z"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_QUOTE},
      PQLToken{PQL_TOKEN_VARIABLE, "y"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  }), QPSParserError);
}

