#include "catch.hpp"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

#include <iostream>

using std::cout;
//
TEST_CASE("Test SP Parser - Procedure") {
  SourceLexer lexer;
  SourceTokenParser parser;

  string ss = "procedure printResults {\n"
              "print flag;\n"
              "print cenX;\n"
              "print cenY;\n"
              "print normSq;\n"
              "while ((x != 0) && (y != 0)) {\n"
              "count = count + 1;\n"
              "cenX = cenX + x;\n"
              "cenY = cenY + y;\n"
              "}"
              "}";

  vector<string> inputList;
  inputList.push_back(ss);

  vector<vector<SourceToken>> tokensList;
  for (string s : inputList) {
    tokensList.push_back(lexer.tokenize(&s));
  }
  for (vector<SourceToken> v : tokensList) {
    auto root = parser.parse(&v);
    REQUIRE(root.getRoot() != nullptr);
    cout << root.getRoot()->toString() << "\n";
  }
}
//
//
//TEST_CASE("Test SP Parser - If") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("if (a > b) then { print good;} else {print bad; print asd; a = b + c  * 9;} ");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//TEST_CASE("Test SP Parser - Statement") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("print abcd;");
//  inputList.push_back("read abcd;");
//  inputList.push_back("asd = abcd + 3;");
//  inputList.push_back("while (i<0) {b = b + 3;};");
//  inputList.push_back("if (count == 0) then {flag = 1;} else {cen = centy;}");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//
//
//TEST_CASE("Test SP Parser - While") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("while (b>b) { print b; abc = a;}");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//TEST_CASE("Test SP Parser - Read") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("read a;");
//  inputList.push_back("read b;");
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//TEST_CASE("Test SP Parser - Print") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("print b;");
//  inputList.push_back("print a;");
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//
//TEST_CASE("Test SP Parser - Assign") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("a = b;");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//
//TEST_CASE("Test SP Parser - Conditional Expressions") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("a < b");
//  inputList.push_back("a < (b)");
//  inputList.push_back("a >= b");
//  inputList.push_back("(a < b) && (b > c)");
//  inputList.push_back("(a < b) || (b > c)");
//  inputList.push_back("((a < b) && (b > c)) || (b <= c)");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//
//TEST_CASE("Test SP Parser - Relational Factor") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("a < b");
//  inputList.push_back("a < (b)");
//  inputList.push_back("a >= b");
//  inputList.push_back("a + b > c + d");
//  inputList.push_back("a + b + c < (d + e)");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
//
//TEST_CASE("Test SP Parser - Expressions") {
//  SourceLexer lexer;
//  SourceTokenParser parser;
//
//  vector<string> inputList;
//  inputList.push_back("a + b * c");
//  inputList.push_back("a");
//  inputList.push_back("a<b");
//  inputList.push_back("(a)");
//  inputList.push_back("a - b");
//  inputList.push_back("a + b");
//  inputList.push_back("a * b");
//  inputList.push_back("(a + b)");
//  inputList.push_back("a + b + c");
//  inputList.push_back("a * b * c");
//  inputList.push_back("a + b * c");
//  inputList.push_back("a * b + c");
//  inputList.push_back("a + (b * c)");
//  inputList.push_back("a * (b + c)");
//  inputList.push_back("(a + b) * c");
//  inputList.push_back("(a * b) + c");
//  inputList.push_back("(a + b + c)");
//  inputList.push_back("(a * b * c)");
//  inputList.push_back("(a + b * c)");
//  inputList.push_back("(a * b + c)");
//  inputList.push_back("(a * b * c)");
//  inputList.push_back("(a * b * c) + (a + b * c)");
//  inputList.push_back("(a * b * c) + (a + b * c)");
//
//  vector<vector<SourceToken>> tokensList;
//  for (string s : inputList) {
//    tokensList.push_back(lexer.tokenize(&s));
//  }
//  for (vector<SourceToken> v : tokensList) {
//    auto root = parser.parse(&v);
//    REQUIRE(root != nullptr);
//    cout << root->toString() << "\n";
//  }
//}
