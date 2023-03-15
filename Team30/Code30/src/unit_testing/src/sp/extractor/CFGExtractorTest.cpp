#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "common/cfg/CFG.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/CFGExtractor.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"
#include "sp/ast/AST.h"

#include <string>
#include <vector>

using std::vector, std::string, std::make_unique;

class CFGExtractorSpy : public CFGExtractor {
 private:
  vector<CFGSPtr> created;

 public:
  explicit CFGExtractorSpy(PkbWriter* pkbWriter): CFGExtractor(pkbWriter) {}
  void addCFGToPKB(CFGSPtr cfg) override {
    created.push_back(cfg);
  }

  vector<CFGSPtr> getCFG() {
    return created;
  }
};

vector<CFGSPtr> executeCFGExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto cfgExtractor = make_unique<CFGExtractorSpy>(&stubby);
  extractors.push_back(cfgExtractor.get());
  ASTPtr ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return cfgExtractor->getCFG();
}

template <class T, class U>
bool findInIterator(T* iteratable, U target) {
  for (auto it = iteratable->begin(); it != iteratable->end(); it++) {
    if (*it == target) {
      return true;
    }
  }
  return false;
}

void assertCFG(CFG* cfg, const vector<pair<int, int>> &links) {
  for (auto x : links) {
    int firstTransformed = cfg->toCFGNode(x.first);
    int secondTransformed = cfg->toCFGNode(x.second);
    REQUIRE(findInIterator(cfg->nextLinksOf(firstTransformed), secondTransformed));
    REQUIRE(findInIterator(cfg->reverseLinksOf(secondTransformed), firstTransformed));
  }

  int fwdCounter = 0;
  for (int i = cfg->getStartingStmtNumber(); cfg->containsStatement(i); i++) {
    auto thisLinks = cfg->nextLinksOf(cfg->toCFGNode(i));
    for (auto it = thisLinks->begin(); it != thisLinks->end(); it++) {
      fwdCounter++;
    }
  }
  REQUIRE(fwdCounter == links.size());

  int reverseCounter = 0;
  for (int i = cfg->getStartingStmtNumber(); cfg->containsStatement(i); i++) {
    auto thisLinks = cfg->reverseLinksOf(cfg->toCFGNode(i));
    for (auto it = thisLinks->begin(); it != thisLinks->end(); it++) {
      reverseCounter++;
    }
  }
  auto thisLinks = cfg->reverseLinksOf(CFG_END_NODE);
  for (auto it = thisLinks->begin(); it != thisLinks->end(); it++) {
    reverseCounter++;
  }
  REQUIRE(reverseCounter == links.size());
}

TEST_CASE("CFGExtractor Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
    {1, 2},
    {2, 3},
    {3, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Statement with If") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "num1 = num1 + 1;"
      "print num1;"
      "} else {"
      "print num2;"
      "}"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
    {1, 2},
    {2, 3},
    {3, 4},
    {4, 5},
    {5, CFG_END_NODE},
    {3, 6},
    {6, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Statement with While loop") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {3, 6},
      {4, 5},
      {5, 3},
      {5, 6},
      {6, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor If in While loop") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "if (num1 == num2) then {"
      "num2 = num2 + 1;"
      "} else {"
      "num2 = 2;"
      "}"
      "num1 = num1 + 1;"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {3, 9},
      {4, 5},
      {4, 6},
      {5, 7},
      {6, 7},
      {7, 8},
      {8, 3},
      {8, 9},
      {9, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor While in If") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}} else {"
      "num2 = num2 + 1;"
      "}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, 5},
      {4, 8},
      {5, 6},
      {6, 4},
      {6, 8},
      {3, 7},
      {7, 8},
      {8, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor While in Else") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "num2 = num2 + 1;"
      "} else {"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},

      {3, 4},
      {4, 8},

      {3, 5},
      {5, 6},
      {5, 8},
      {6, 7},
      {7, 5},
      {7, 8},

      {8, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Triple-While Chain") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "  while (num1 > 0) {"
      "    while (num2 > 0) {"
      "      num2 = num2 + 1;"
      "    }"
      "    num1 = num1 + 1;"
      "  }"
      "  print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {3, 9},
      {4, 5},
      {4, 8},
      {5, 6},
      {5, 7},
      {6, 5},
      {6, 7},
      {7, 8},
      {7, 4},
      {8, 9},
      {8, 3},
      {9, CFG_END_NODE}
  });
}

TEST_CASE("CFG Triple-If Chain") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "  if (num1 > 0) then {"
      "    if (num2 > 0) then {"
      "      num2 = num2 + 1;"
      "    } else {"
      "      print num1;"
      "  } num1 = num1 + 1;"
      "  } else {"
      "    print num2;"
      "} print num1;"
      "} else {"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, 5},

      {5, 6},
      {6, 8},

      {5, 7},
      {7, 8},

      {8, 10},
      {4, 9},
      {9, 10},

      {10, 12},
      {3, 11},
      {11, 12},

      {12, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Two Procedures with Simple Statement list") {
  string input =
      "procedure simple {"
      "read num0;"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure simpleTwo {"
      "read num4;"
      "read num5;"
      "read num6;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 2);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, CFG_END_NODE}
  });

  assertCFG(setofCFGs[1].get(), {
      {5, 6},
      {6, 7},
      {7, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Three Procedures with Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure simpleTwo {"
      "read num4;"
      "read num5;"
      "read num6;"
      "}"
      "procedure simpleThree {"
      "read num7;"
      "read num8;"
      "read num9;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 3);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, CFG_END_NODE}
  });

  assertCFG(setofCFGs[1].get(), {
      {4, 5},
      {5, 6},
      {6, CFG_END_NODE}
  });

  assertCFG(setofCFGs[2].get(), {
      {7, 8},
      {8, 9},
      {9, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Two Procedures with complex statements") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "  if (num1 > 0) then {"
      "    if (num2 > 0) then {"
      "      num2 = num2 + 1;"
      "    } else {"
      "      print num1;"
      "  } num1 = num1 + 1;"
      "  } else {"
      "    print num2;"
      "} print num1;"
      "} else {"
      "print num1;"
      "}"
      "print num2;"
      "}"
      "procedure simpleTwo {"
      "read num3;"
      "read num4;"
      "while (num3 < num4) {"
      "  while (num3 > 0) {"
      "    while (num4 > 0) {"
      "      num4 = num4 + 1;"
      "    }"
      "    num = num4 + 1;"
      "  }"
      "  print num4;"
      "}"
      "print num4;"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 2);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, 5},
      {5, 6},
      {6, 8},
      {5, 7},
      {7, 8},
      {8, 10},
      {4, 9},
      {9, 10},
      {10, 12},
      {3, 11},
      {11, 12},
      {12, CFG_END_NODE}
  });

  assertCFG(setofCFGs[1].get(), {
      {13, 14},
      {14, 15},
      {15, 16},
      {15, 21},
      {16, 17},
      {16, 20},
      {17, 18},
      {17, 19},
      {18, 17},
      {18, 19},
      {19, 16},
      {19, 20},
      {20, 15},
      {20, 21},
      {21, CFG_END_NODE}
  });
}

TEST_CASE("CFGExtractor Nested While") {
  string input =
      "procedure simple {"
      "read num1;"
      "while (x == 2) {"
      "  while (y == 3) {"
      "    x = 4;"
      "  }"
      "}"
      "}";

  vector<CFGSPtr> setofCFGs = executeCFGExtractor(input);
  REQUIRE(setofCFGs.size() == 1);
  assertCFG(setofCFGs[0].get(), {
      {1, 2},
      {2, 3},
      {2, CFG_END_NODE},
      {3, 4},
      {3, CFG_END_NODE},
      {3, 2},
      {4, 3},
      {4, 2},
      {4, CFG_END_NODE}
  });
}
