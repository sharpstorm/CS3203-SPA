#include "catch.hpp"

#include <vector>
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "PQLContextTestUtils.h"
#include "qps/parser/token_parser/QueryTokenParser.h"
#include "qps/parser/lexer/QueryLexerFactory.h"

using std::vector, std::make_unique;

void testStringParsing(string query) {
  SourceParserStub exprParser;
  QueryLexerFactory lexerFactory;
  auto tokens = lexerFactory.makeLexer(&query)->getTokenStream();
  QueryTokenParser parser(&exprParser, tokens.get());
  parser.build();
}

TEST_CASE("Test QueryTokenParseState Stage Valid Flows") {
  auto dummyStream = vector<PQLToken>{
    PQLToken(PQL_TOKEN_SELECT)
  };

  testStringParsing("procedure p; Select p");
  testStringParsing("Select BOOLEAN");
  testStringParsing("procedure p; Select p such that Follows(1, 2)");
  testStringParsing("procedure p; assign a; Select p pattern a(_, _)");
  testStringParsing("procedure p; assign a; Select p such that Follows(1, 2) pattern a(_, _)");
  testStringParsing("procedure p; assign a; Select p pattern a(_, _) such that Follows(1, 2)");
  testStringParsing("assign a; Select a with 1 = 1");
  testStringParsing("assign a; Select a with 1 = a.stmt#");
  testStringParsing("assign a; Select a with a.stmt# = 1");
  testStringParsing("variable v; Select v with \"x\" = \"x\"");
  testStringParsing("variable v; Select v with \"x\" = v.varName");
  testStringParsing("variable v; Select v with v.varName = \"x\"");
  testStringParsing("assign a; variable v; Select a such that Uses(a,v) with a.stmt# = 1");
  testStringParsing("assign a; variable v; Select a with a.stmt# = 1 such that Uses(a,v)");
  // Disabled for now - Cat 3 cases
//  testStringParsing("assign a; Select a with a.stmt# = a.stmt#");
//  testStringParsing("variable v; Select v with v.varName = v.varName");
}

TEST_CASE("Test QueryTokenParseState Stage Invalid Flows") {
  REQUIRE_THROWS_AS(testStringParsing("such that"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("Follows(1, 2)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("pattern"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("a(_, _)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing(""), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; such that"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Follows(1, 2)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; pattern"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; a(_, _)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a;"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a assign a2;"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a Follows(1, 2)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a a(_, _)"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that assign a2"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that Select a"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that pattern"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that a(_, _)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that Follows(1, 2) assign a2"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that Follows(1, 2) Select a"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a such that Follows(1, 2) a(_, _)"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern assign a2"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern Select a"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern such that"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern Follows(1,1)"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern a(_,_) assign a2"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern a(_,_) Select a"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a pattern a(_,_) Follows(1, 2)"), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a with = 1"), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a with = "), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(testStringParsing("assign a; Select a with 1 = "), QPSParserSyntaxError);
}
