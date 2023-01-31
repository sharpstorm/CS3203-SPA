#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "catch.hpp"
#include "qps/parser/QueryLexer.h"

using std::vector, std::string, std::cout;

void testLexing(string testCase, vector<string> expected) {
  QueryLexer lexer;
  vector<string>* result = lexer.getTokenStream(&testCase);
  REQUIRE(result->size() == expected.size());
  REQUIRE(equal(expected.begin(), expected.end(), result->begin(), result->end()));
}

TEST_CASE("Test QPS Lexer Ignore spaces") {
  testLexing(" a  b ", vector<string>{"a", "b"});
  testLexing(" a  b dd e fff", vector<string>{"a", "b", "dd", "e", "fff"});

  testLexing("\ta\tb\t", vector<string>{"a", "b"});
  testLexing("\na\nb\n", vector<string>{"a", "b"});
  testLexing("\r\na\r\nb\r\n", vector<string>{"a", "b"});
}

