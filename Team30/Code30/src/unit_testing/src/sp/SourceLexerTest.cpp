#include "catch.hpp"

#include <vector>
#include "sp/lexer/SourceLexer.h"
#include "sp/common/SourceToken.h"

using std::vector;

TEST_CASE("Test Source Lexer") {
  SourceLexer lexer;

  string input = "procedure q {\n"
                 "while(i!=0) {\n"
                 "  x = x + 2;\n"
                 "}}";
  vector<SourceToken> result = lexer.tokenize(&input);
  vector<SourceToken> expected = vector<SourceToken>{
    SourceToken(SIMPLE_TOKEN_KEYWORD_PROCEDURE, ""),
    SourceToken(SIMPLE_TOKEN_VARIABLE, "q"),
    SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_LEFT, ""),
    SourceToken(SIMPLE_TOKEN_KEYWORD_WHILE, ""),
    SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
    SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
    SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
    SourceToken(SIMPLE_TOKEN_INTEGER, "0"),
    SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
    SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_LEFT, ""),
    SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
    SourceToken(SIMPLE_TOKEN_ASSIGN, ""),
    SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
    SourceToken(SIMPLE_TOKEN_PLUS, ""),
    SourceToken(SIMPLE_TOKEN_INTEGER, "2"),
    SourceToken(SIMPLE_TOKEN_SEMICOLON, ""),
    SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT, ""),
    SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT, ""),
  };

  REQUIRE(equal(expected.begin(), expected.end(), result.begin(), result.end()));
}
