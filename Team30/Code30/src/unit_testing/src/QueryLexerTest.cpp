#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "catch.hpp"
#include "qps/parser/lexer/QueryLexer.h"

using std::vector, std::string, std::cout;

void testPQLLexing(string testCase, vector<PQLToken> expected) {
  QueryLexer lexer;
  vector<PQLToken>* result = lexer.getTokenStream(&testCase);
  REQUIRE(result->size() == expected.size());
  REQUIRE(equal(expected.begin(), expected.end(), result->begin(), result->end()));
}

void testPQLLexSingleToken(string testCase, PQLTokenType expected) {
  QueryLexer lexer;
  vector<PQLToken>* result = lexer.getTokenStream(&testCase);
  REQUIRE(result->size() == 1);
  REQUIRE(result->at(0).type == expected);
}

void testPQLDeclaration(string testCase, PQLTokenType expectedToken, PQLToken expectedArg) {
  QueryLexer lexer;
  vector<PQLToken>* result = lexer.getTokenStream(&testCase);
  testPQLLexing(testCase, vector<PQLToken>{
    PQLToken{ expectedToken },
    expectedArg,
  });
}

TEST_CASE("Test QPS Lexer Ignore spaces") {
  vector<PQLToken> expected = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_STRING, "bbb"},
      PQLToken{PQL_TOKEN_STRING, "cc"},
  };

  testPQLLexing("a  bbb cc", expected);
  testPQLLexing(" a bbb cc ", expected);
  testPQLLexing("\t a\t bbb\t\tcc \t", expected);
  testPQLLexing("\n a\n bbb\n\r\ncc \n", expected);
  testPQLLexing("\n a\n b\rbb\n\ncc \n", expected);
}

TEST_CASE("Test QPS Lexer Declarations") {
  PQLToken stringToken = PQLToken{PQL_TOKEN_STRING, "a"};

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
  PQLToken stringToken = PQLToken{PQL_TOKEN_STRING, "a"};

  testPQLDeclaration("Select a", PQL_TOKEN_SELECT, stringToken);
  testPQLDeclaration("pattern a", PQL_TOKEN_PATTERN, stringToken);
  testPQLLexing("such that", vector<PQLToken>{
      PQLToken{PQL_TOKEN_SUCH},
      PQLToken{PQL_TOKEN_THAT}
  });
}

TEST_CASE("Test QPS Lexer Symbols") {
  vector<PQLToken> symbolSet = vector<PQLToken>{
      PQLToken{PQL_TOKEN_SEMICOLON},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_PERIOD},
      PQLToken{PQL_TOKEN_UNDERSCORE},
      PQLToken{PQL_TOKEN_QUOTE},
  };

  testPQLLexing(";(),._\"", symbolSet);
  testPQLLexing("; ( ) , . _ \" ", symbolSet);
}

TEST_CASE("Test QPS Lexer Relationships") {
  testPQLLexSingleToken("Follows", PQL_TOKEN_FOLLOWS);
  testPQLLexSingleToken("Parent", PQL_TOKEN_PARENT);
  testPQLLexSingleToken("Uses", PQL_TOKEN_USES);
  testPQLLexSingleToken("Modifies", PQL_TOKEN_MODIFIES);
}

TEST_CASE("Test Lex full query ") {
  testPQLLexing("assign a,b; Select a such that Follows(a, b)", vector<PQLToken>{
    PQLToken{PQL_TOKEN_ASSIGN},
    PQLToken{PQL_TOKEN_STRING, "a"},
    PQLToken{PQL_TOKEN_COMMA},
    PQLToken{PQL_TOKEN_STRING, "b"},
    PQLToken{PQL_TOKEN_SEMICOLON},
    PQLToken{PQL_TOKEN_SELECT},
    PQLToken{PQL_TOKEN_STRING, "a"},
    PQLToken{PQL_TOKEN_SUCH},
    PQLToken{PQL_TOKEN_THAT},
    PQLToken{PQL_TOKEN_FOLLOWS},
    PQLToken{PQL_TOKEN_BRACKET_OPEN},
    PQLToken{PQL_TOKEN_STRING, "a"},
    PQLToken{PQL_TOKEN_COMMA},
    PQLToken{PQL_TOKEN_STRING, "b"},
    PQLToken{PQL_TOKEN_BRACKET_CLOSE},
  });
}
