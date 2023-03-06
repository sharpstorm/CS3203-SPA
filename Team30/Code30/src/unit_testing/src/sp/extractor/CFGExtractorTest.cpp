#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/ast/CFG.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/CFGExtractor.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"
#include "sp/ast/AST.h"

using std::vector, std::string, std::pair, std::make_unique;

vector<CFG>* executeCFGExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<Extractor*> extractors;
  auto cfgExtractor = make_unique<CFGExtractor>(&stubby);
  extractors.push_back(cfgExtractor.get());
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, &extractors);
  return cfgExtractor->getSetOfCFGs();
}

TEST_CASE("CFGExtractor Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  vector<CFG>* setofCFGs = executeCFGExtractor(input);

  REQUIRE(!setofCFGs->empty());
}
