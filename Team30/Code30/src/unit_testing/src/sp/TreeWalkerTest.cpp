#include "catch.hpp"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"
#include "sp/SpFacade.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

#include <iostream>

using std::cout;

TEST_CASE("Tree Walker") {
  string ss = "procedure printResults {\n"
              "print flag;\n"
              "print cenX;\n"
              "print cenY;\n"
              "print normSq;\n"
              "if (i < 3) then { b = 3 + 3; } else { c = 3 + 2;}"
              "read abc;"
              "}";

  SourceParser parser;
  TreeWalker treeWalker;
  AST ast = parser.parseSource(ss);
  PKB pkb;

//  PkbWriter pkbWriter(&pkb)
//  shared_ptr<Extractor> f = shared_ptr<Extractor>(new FollowsExtractor);
//  ast.getRoot()->getChildren()[0]->accept(f);


}

