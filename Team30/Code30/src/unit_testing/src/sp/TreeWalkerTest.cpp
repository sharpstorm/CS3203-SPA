#include "catch.hpp"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"
#include "sp/SpFacade.h"
#include "sp/extractor/TreeWalker.h"

#include <iostream>

using std::cout;

TEST_CASE("Tree Walker") {
  string ss = "procedure printResults {\n"
              "print flag;\n"
              "print cenX;\n"
              "print cenY;\n"
              "print normSq;\n"
              "if (i < 3) then { b = 3 + 3; } else { c = 3 + 2;}"
              "}";

  SourceParser parser;
  TreeWalker treeWalker;
  AST ast = parser.parseSource(ss);
  treeWalker.walkAST(ast);


}

