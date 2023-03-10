#include "sp/ast/AST.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/ProcedureRangeExtractor.h"

using std::vector, std::string, std::pair, std::make_unique;

vector<triple<string, int, int>> executeProcedureRangeExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto procedureRangeExtractor = make_unique<ProcedureRangeExtractor>(&stubby);
  extractors.push_back(procedureRangeExtractor.get());
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, &extractors);
  return stubby.procedureRangeStore;
}

TEST_CASE("ProcedureRangeExtractor Single Procedure") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  Util u;
  vector<triple<string, int, int>> v = executeProcedureRangeExtractor(input);
  REQUIRE(u.contains(v, "simple", 1, 3));
}

TEST_CASE("ProcedureRangeExtractor Multiple Procedure") {
  string input =
      "procedure p1 {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure p2 {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure p3 {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  Util u;
  vector<triple<string, int, int>> v = executeProcedureRangeExtractor(input);
  REQUIRE(u.contains(v, "p1", 1, 3));
  REQUIRE(u.contains(v, "p2", 4, 6));
  REQUIRE(u.contains(v, "p3", 7, 9));
}

TEST_CASE("ProcedureRangeExtractor More Complex Multiple Procedure") {
  string input = "procedure proc1 {\n"
                 "  if (a < 1) then { "
                 "    read x; "
                 "  } else {"
                 "    while (a < 3) {"
                 "      b = 1;"
                 "    }"
                 "  }"
                 "}"
                 "procedure proc2 {\n"
                 "  print abc;"
                 "}"
                 "procedure proc3 {\n"
                 "  if (a < 1) then { read x; }"
                 "  else {"
                 "      b = 1;"
                 "  }"
                 "}";

  Util u;
  vector<triple<string, int, int>> v = executeProcedureRangeExtractor(input);
  REQUIRE(u.contains(v, "proc1", 1, 4));
  REQUIRE(u.contains(v, "proc2", 5, 5));
  REQUIRE(u.contains(v, "proc3", 6, 8));
}


