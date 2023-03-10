#include <string>
#include <vector>
#include <iostream>

#include "catch.hpp"
#include "qps/parser/lexer/QueryLexer.h"
#include "qps/parser/lexer/QueryLexerFactory.h"

using std::vector, std::string, std::cout;

void testPQLLexing(string testCase, vector<PQLToken> expected) {
  QueryLexerFactory lexFactory;
  PQLTokenStreamPtr result = lexFactory.makeLexer(&testCase)->getTokenStream();
  REQUIRE(result->size() == expected.size());
  REQUIRE(equal(expected.begin(), expected.end(), result->begin(), result->end()));
}

void testPQLLexSingleToken(string testCase, PQLTokenType expected) {
  QueryLexerFactory lexFactory;
  PQLTokenStreamPtr result = lexFactory.makeLexer(&testCase)->getTokenStream();
  REQUIRE(result->size() == 1);
  REQUIRE(result->at(0).getType() == expected);
}

void testPQLLexSingleToken(string testCase, PQLToken expected) {
  QueryLexerFactory lexFactory;
  PQLTokenStreamPtr result = lexFactory.makeLexer(&testCase)->getTokenStream();
  REQUIRE(result->size() == 1);
  INFO(result->at(0).getType());
  REQUIRE(result->at(0) == expected);
}

void testPQLDeclaration(string testCase, PQLTokenType expectedToken, PQLToken expectedArg) {
  QueryLexerFactory lexFactory;
  PQLTokenStreamPtr result = lexFactory.makeLexer(&testCase)->getTokenStream();
  testPQLLexing(testCase, vector<PQLToken>{
    PQLToken(expectedToken),
    expectedArg,
  });
}

TEST_CASE("Test QPS Lexer Ignore spaces") {
  vector<PQLToken> expected = vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "a"),
      PQLToken(PQL_TOKEN_STRING, "bbb"),
      PQLToken(PQL_TOKEN_STRING, "cc"),
  };

  testPQLLexing("a  bbb cc", expected);
  testPQLLexing(" a bbb cc ", expected);
  testPQLLexing("\t a\t bbb\t\tcc \t", expected);
  testPQLLexing("\n a\n bbb\n\r\ncc \n", expected);
  testPQLLexing("\n a\n b\rbb\n\ncc \n", expected);
}

TEST_CASE("Test QPS Lexer Declarations") {
  PQLToken stringToken = PQLToken(PQL_TOKEN_STRING, "a");

  testPQLDeclaration("stmt a", PQL_TOKEN_STMT, stringToken);
  testPQLDeclaration("read a", PQL_TOKEN_READ, stringToken);
  testPQLDeclaration("print a", PQL_TOKEN_PRINT, stringToken);
  testPQLDeclaration("call a", PQL_TOKEN_CALL, stringToken);
  testPQLDeclaration("while a", PQL_TOKEN_WHILE, stringToken);
  testPQLDeclaration("if a", PQL_TOKEN_IF, stringToken);
  testPQLDeclaration("assign a", PQL_TOKEN_ASSIGN, stringToken);
  testPQLDeclaration("variable a", PQL_TOKEN_VARIABLE, stringToken);
  testPQLDeclaration("constant a", PQL_TOKEN_CONSTANT, stringToken);
  testPQLDeclaration("procedure a", PQL_TOKEN_PROCEDURE, stringToken);
}

TEST_CASE("Test QPS Lexer Query keywords") {
  PQLToken stringToken = PQLToken(PQL_TOKEN_STRING, "a");

  testPQLDeclaration("Select a", PQL_TOKEN_SELECT, stringToken);
  testPQLDeclaration("pattern a", PQL_TOKEN_PATTERN, stringToken);
  testPQLLexing("such that", vector<PQLToken>{
      PQLToken(PQL_TOKEN_SUCH),
      PQLToken(PQL_TOKEN_THAT)
  });
}

TEST_CASE("Test QPS Lexer Symbols") {
  vector<PQLToken> symbolSet = vector<PQLToken>{
      PQLToken(PQL_TOKEN_SEMICOLON),
      PQLToken(PQL_TOKEN_BRACKET_OPEN),
      PQLToken(PQL_TOKEN_BRACKET_CLOSE),
      PQLToken(PQL_TOKEN_COMMA),
      PQLToken(PQL_TOKEN_PERIOD),
      PQLToken(PQL_TOKEN_UNDERSCORE),
      PQLToken(PQL_TOKEN_LITERAL, "+"),
      PQLToken(PQL_TOKEN_ASTRIX),
      PQLToken(PQL_TOKEN_EQUALS)
  };

  testPQLLexing(";(),._\"+\"*=", symbolSet);
  testPQLLexing("; ( ) , . _ \"+\" * =", symbolSet);
}

TEST_CASE("Test QPS Lexer Relationships") {
  testPQLLexSingleToken("Follows", PQL_TOKEN_FOLLOWS);
  testPQLLexSingleToken("Parent", PQL_TOKEN_PARENT);
  testPQLLexSingleToken("Uses", PQL_TOKEN_USES);
  testPQLLexSingleToken("Modifies", PQL_TOKEN_MODIFIES);
  testPQLLexSingleToken("Calls", PQL_TOKEN_CALLS);
}

TEST_CASE("Test QPS Lexer Literals") {
  testPQLLexSingleToken("\"abc\"", PQLToken(PQL_TOKEN_STRING_LITERAL, "abc"));
  testPQLLexSingleToken("\"abc123\"", PQLToken(PQL_TOKEN_STRING_LITERAL, "abc123"));
  testPQLLexSingleToken("\"a1bc\"", PQLToken(PQL_TOKEN_STRING_LITERAL, "a1bc"));
  testPQLLexSingleToken("\"123\"", PQLToken(PQL_TOKEN_LITERAL, "123"));
  testPQLLexSingleToken("\"abc abc\"", PQLToken(PQL_TOKEN_LITERAL, "abc abc"));
  testPQLLexSingleToken("\"a+b\"", PQLToken(PQL_TOKEN_LITERAL, "a + b"));
  testPQLLexSingleToken("\"a + b    + c\"", PQLToken(PQL_TOKEN_LITERAL, "a + b + c"));
}

TEST_CASE("Test QPS Lexer full query") {
  testPQLLexing("assign a,b; Select a such that Follows(a, b)", vector<PQLToken>{
    PQLToken(PQL_TOKEN_ASSIGN),
    PQLToken(PQL_TOKEN_STRING, "a"),
    PQLToken(PQL_TOKEN_COMMA),
    PQLToken(PQL_TOKEN_STRING, "b"),
    PQLToken(PQL_TOKEN_SEMICOLON),
    PQLToken(PQL_TOKEN_SELECT),
    PQLToken(PQL_TOKEN_STRING, "a"),
    PQLToken(PQL_TOKEN_SUCH),
    PQLToken(PQL_TOKEN_THAT),
    PQLToken(PQL_TOKEN_FOLLOWS),
    PQLToken(PQL_TOKEN_BRACKET_OPEN),
    PQLToken(PQL_TOKEN_STRING, "a"),
    PQLToken(PQL_TOKEN_COMMA),
    PQLToken(PQL_TOKEN_STRING, "b"),
    PQLToken(PQL_TOKEN_BRACKET_CLOSE),
  });
}

TEST_CASE("Test QPS Lexer Case Sensitivity") {
  testPQLLexing("Stmt Read Print Call While If Assign Variable Constant Procedure", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "Stmt"),
      PQLToken(PQL_TOKEN_STRING, "Read"),
      PQLToken(PQL_TOKEN_STRING, "Print"),
      PQLToken(PQL_TOKEN_STRING, "Call"),
      PQLToken(PQL_TOKEN_STRING, "While"),
      PQLToken(PQL_TOKEN_STRING, "If"),
      PQLToken(PQL_TOKEN_STRING, "Assign"),
      PQLToken(PQL_TOKEN_STRING, "Variable"),
      PQLToken(PQL_TOKEN_STRING, "Constant"),
      PQLToken(PQL_TOKEN_STRING, "Procedure")
  });

  testPQLLexing("sElect Such That Pattern", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "sElect"),
      PQLToken(PQL_TOKEN_STRING, "Such"),
      PQLToken(PQL_TOKEN_STRING, "That"),
      PQLToken(PQL_TOKEN_STRING, "Pattern")
  });
}

TEST_CASE("Test QPS Lexer Literal Handling") {
  testPQLLexing("Modifies(1, \"   b    \")", vector<PQLToken>{
      PQLToken(PQL_TOKEN_MODIFIES, "Modifies"),
      PQLToken(PQL_TOKEN_BRACKET_OPEN),
      PQLToken(PQL_TOKEN_INTEGER, "1"),
      PQLToken(PQL_TOKEN_COMMA),
      PQLToken(PQL_TOKEN_STRING_LITERAL, "b"),
      PQLToken(PQL_TOKEN_BRACKET_CLOSE)
  });

  testPQLLexing("a(_, \" a b   \")", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "a"),
      PQLToken(PQL_TOKEN_BRACKET_OPEN),
      PQLToken(PQL_TOKEN_UNDERSCORE),
      PQLToken(PQL_TOKEN_COMMA),
      PQLToken(PQL_TOKEN_LITERAL, "a b"),
      PQLToken(PQL_TOKEN_BRACKET_CLOSE)
  });

  testPQLLexing("a(_, \" a\tb + c   \")", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "a"),
      PQLToken(PQL_TOKEN_BRACKET_OPEN),
      PQLToken(PQL_TOKEN_UNDERSCORE),
      PQLToken(PQL_TOKEN_COMMA),
      PQLToken(PQL_TOKEN_LITERAL, "a b + c"),
      PQLToken(PQL_TOKEN_BRACKET_CLOSE)
  });
}

TEST_CASE("Test QPS Lexer Attributes") {
  testPQLLexing("a1.stmt#", vector<PQLToken>{
    PQLToken(PQL_TOKEN_STRING, "a1"),
    PQLToken(PQL_TOKEN_PERIOD),
    PQLToken(PQL_TOKEN_STMT),
    PQLToken(PQL_TOKEN_NUMBER_SIGN)
  });

  testPQLLexing("p.procName", vector<PQLToken>{
    PQLToken(PQL_TOKEN_STRING, "p"),
    PQLToken(PQL_TOKEN_PERIOD),
    PQLToken(PQL_TOKEN_PROC_NAME)
  });

  testPQLLexing("v.varName", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "v"),
      PQLToken(PQL_TOKEN_PERIOD),
      PQLToken(PQL_TOKEN_VAR_NAME)
  });

  testPQLLexing("c.value", vector<PQLToken>{
      PQLToken(PQL_TOKEN_STRING, "c"),
      PQLToken(PQL_TOKEN_PERIOD),
      PQLToken(PQL_TOKEN_VALUE)
  });
}

TEST_CASE("Test QPS Lexer Attribute Compare Handling") {
  testPQLLexing("1 = 1", vector<PQLToken>{
    PQLToken(PQL_TOKEN_INTEGER, "1"),
    PQLToken(PQL_TOKEN_EQUALS),
    PQLToken(PQL_TOKEN_INTEGER, "1")
  });

  testPQLLexing("a1.stmt# = 1", vector<PQLToken>{
    PQLToken(PQL_TOKEN_STRING, "a1"),
    PQLToken(PQL_TOKEN_PERIOD),
    PQLToken(PQL_TOKEN_STMT),
    PQLToken(PQL_TOKEN_NUMBER_SIGN),
    PQLToken(PQL_TOKEN_EQUALS),
    PQLToken(PQL_TOKEN_INTEGER, "1")
  });

  testPQLLexing("1 = a1.stmt#", vector<PQLToken>{
      PQLToken(PQL_TOKEN_INTEGER, "1"),
      PQLToken(PQL_TOKEN_EQUALS),
      PQLToken(PQL_TOKEN_STRING, "a1"),
      PQLToken(PQL_TOKEN_PERIOD),
      PQLToken(PQL_TOKEN_STMT),
      PQLToken(PQL_TOKEN_NUMBER_SIGN)
  });
}

