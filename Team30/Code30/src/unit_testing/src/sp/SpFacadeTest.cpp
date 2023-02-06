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
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
//  auto pkb = std::make_unique<PKB>();
//  auto pkbWriter = std::make_unique<PkbWriter>(pkb.get());
  SpFacade spFacade = SpFacade();
  spFacade.parseSource("test.txt", &pkbWriter);
  unordered_set<int> :: iterator itr;
  unordered_set<int> f = pkb.followsStore->getByFirstArgT(3);
  for (itr = f.begin(); itr != f.end(); itr++) {
    cout << (*itr) << "\n";
  }

}

TEST_CASE("File Reader") {
  FileReader reader;
  string ss = reader.readFromFile("test.txt");
  cout << ss << "\n";
}

