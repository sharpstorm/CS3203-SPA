#include "catch.hpp"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"
#include "sp/SpFacade.h"

#include <iostream>

using std::cout;



TEST_CASE("Parser") {
  string ss = "procedure printResults {\n"
              "print flag;\n"
              "print cenX;\n"
              "print cenY;\n"
              "print normSq;\n"
              "}";

  SourceParser parser;
  AST ast = parser.parseSource(ss);
  cout << ast.getRoot()->toString() << "\n";


}


TEST_CASE("SP facade") {
  SpFacade spFacade = SpFacade();
  spFacade.parseSource("test.txt");
}

TEST_CASE("File Reader") {
  FileReader reader;
  string ss = reader.readFromFile("test.txt");
  cout << ss << "\n";
}

