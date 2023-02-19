#include "catch.hpp"
#include <vector>
#include "sp/lexer/SourceLexer.h"
#include "sp/common/SourceToken.h"

using std::vector;

TEST_CASE("Test Source Lexer General") {
  SourceLexer lexer;

  string input = "procedure q {\n"
                 "while(i!=0) {\n"
                 "  x = x + 2;\n"
                 "}}";
  SourceTokenStreamPtr result = lexer.tokenize(&input);
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

  REQUIRE(equal(expected.begin(), expected.end(),
                result->begin(), result->end()));
}

TEST_CASE("Test Source Condition") {
  SourceLexer lexer;

  string input = "procedure q {\n"
                 "while(i != 0 && !(i == 0) && i < 0 || i <= 0 || i >= 0 && i > 0) {\n"
                 "  helloIamAverylongLongUnder100CharacterString = aaaa + 2;\n"
                 "}}";
  SourceTokenStreamPtr result = lexer.tokenize(&input);
  vector<SourceToken> expected = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_KEYWORD_PROCEDURE, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "q"),
      SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_KEYWORD_WHILE, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),

      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),

      SourceToken(SIMPLE_TOKEN_AND, ""),
      SourceToken(SIMPLE_TOKEN_NOT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),

      SourceToken(SIMPLE_TOKEN_AND, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_LT, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),

      SourceToken(SIMPLE_TOKEN_OR, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_LTE, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),

      SourceToken(SIMPLE_TOKEN_OR, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_GTE, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),

      SourceToken(SIMPLE_TOKEN_AND, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "i"),
      SourceToken(SIMPLE_TOKEN_GT, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "0"),

      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "helloIamAverylongLongUnder100CharacterString"),
      SourceToken(SIMPLE_TOKEN_ASSIGN, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "aaaa"),
      SourceToken(SIMPLE_TOKEN_PLUS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "2"),
      SourceToken(SIMPLE_TOKEN_SEMICOLON, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT, ""),
  };

  REQUIRE(equal(expected.begin(), expected.end(),
                result->begin(), result->end()));
}
