#include "catch.hpp"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"
#include "common/Types.h"

typedef unordered_set<CFGNode> CFGNodeSet;
typedef pair_set<CFGNode, CFGNode> CFGNodePairSet;

void setWalkCallback(CFGNodeSet* state, CFGNode node) {
  if (state->find(node) != state->end()) {
    FAIL("Same node callback twice");
  }
  state->insert(node);
}

void pairWalkCallback(CFGNodePairSet* state,
                      CFGNode nodeLeft,
                      CFGNode nodeRight) {
  if (state->find({nodeLeft, nodeRight}) != state->end()) {
    FAIL("Same node callback twice");
  }
  state->insert({ nodeLeft, nodeRight });
}

void assertWalkFrom(CFGWalker* walker, CFGNode from, CFGNodeSet set) {
  CFGNodeSet nodes;
  walker->walkFrom<CFGNodeSet, setWalkCallback>(from, &nodes);
  REQUIRE(nodes == set);
}

void assertWalkTo(CFGWalker* walker, CFGNode to, CFGNodeSet set) {
  CFGNodeSet nodes;
  walker->walkTo<CFGNodeSet, setWalkCallback>(to, &nodes);
  REQUIRE(nodes == set);
}

void assertWalkAll(CFGWalker* walker, CFGNodePairSet set) {
  CFGNodePairSet nodes;
  walker->walkAll<CFGNodePairSet, pairWalkCallback>(&nodes);
  REQUIRE(nodes == set);
}

/*
 * 0 |  x = 1;
 * 1 |  y = 2;
 * 2 |  z = 3;
 */
CFG getLinearCFG() {
  CFG cfg("Linear", 1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 3);
  cfg.addLink(3, CFG_END_NODE);

  return cfg;
}

TEST_CASE("CFG Linear Static Walk") {
  auto cfg = getLinearCFG();
  CFGWalker walker(&cfg);

  REQUIRE(walker.walkStatic(0, 1));
  REQUIRE(walker.walkStatic(0, 2));
  REQUIRE(walker.walkStatic(0, 3));
  REQUIRE(walker.walkStatic(1, 2));
  REQUIRE(walker.walkStatic(1, 3));
  REQUIRE(walker.walkStatic(2, 3));

  REQUIRE_FALSE(walker.walkStatic(0, 0));
  REQUIRE_FALSE(walker.walkStatic(1, 1));
  REQUIRE_FALSE(walker.walkStatic(2, 2));
  REQUIRE_FALSE(walker.walkStatic(3, 3));

  REQUIRE_FALSE(walker.walkStatic(1, 0));
  REQUIRE_FALSE(walker.walkStatic(2, 0));
  REQUIRE_FALSE(walker.walkStatic(3, 0));
  REQUIRE_FALSE(walker.walkStatic(2, 1));
  REQUIRE_FALSE(walker.walkStatic(3, 1));
  REQUIRE_FALSE(walker.walkStatic(3, 2));
}

TEST_CASE("CFG Linear Set Walk") {
  auto cfg = getLinearCFG();
  CFGWalker walker(&cfg);

  assertWalkFrom(&walker, 0, {1, 2, 3});
  assertWalkFrom(&walker, 1, {2, 3});
  assertWalkFrom(&walker, 2, {3});
  assertWalkFrom(&walker, 3, {});

  assertWalkTo(&walker, 0, {});
  assertWalkTo(&walker, 1, {0});
  assertWalkTo(&walker, 2, {0, 1});
  assertWalkTo(&walker, 3, {0, 1, 2});

  assertWalkAll(&walker, {
      {0, 1},
      {0, 2},
      {0, 3},
      {1, 2},
      {1, 3},
      {2, 3}
  });
}

/*
 * 0 |  x = 1;
 * 1 |  if (x != 1) then {
 * 2 |    x = 2;
 *   |  } else {
 * 3 |    x = 3;
 *   |  }
 */

CFG getSimpleIfCFG() {
  CFG cfg("SimpleIf", 1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(1, 3);
  cfg.addLink(2, CFG_END_NODE);
  cfg.addLink(3, CFG_END_NODE);

  return cfg;
}

TEST_CASE("CFG Simple If Static Walk") {
  auto cfg = getSimpleIfCFG();
  CFGWalker walker(&cfg);

  REQUIRE(walker.walkStatic(0, 1));
  REQUIRE(walker.walkStatic(0, 2));
  REQUIRE(walker.walkStatic(0, 3));
  REQUIRE(walker.walkStatic(1, 2));
  REQUIRE(walker.walkStatic(1, 3));

  // Special for If
  REQUIRE_FALSE(walker.walkStatic(2, 3));

  REQUIRE_FALSE(walker.walkStatic(0, 0));
  REQUIRE_FALSE(walker.walkStatic(1, 1));
  REQUIRE_FALSE(walker.walkStatic(2, 2));
  REQUIRE_FALSE(walker.walkStatic(3, 3));

  REQUIRE_FALSE(walker.walkStatic(1, 0));
  REQUIRE_FALSE(walker.walkStatic(2, 0));
  REQUIRE_FALSE(walker.walkStatic(3, 0));
  REQUIRE_FALSE(walker.walkStatic(2, 1));
  REQUIRE_FALSE(walker.walkStatic(3, 1));
  REQUIRE_FALSE(walker.walkStatic(3, 2));
}

TEST_CASE("CFG Simple If Set Walk") {
  auto cfg = getSimpleIfCFG();
  CFGWalker walker(&cfg);

  assertWalkFrom(&walker, 0, {1, 2, 3});
  assertWalkFrom(&walker, 1, {2, 3});
  assertWalkFrom(&walker, 2, {});
  assertWalkFrom(&walker, 3, {});

  assertWalkTo(&walker, 0, {});
  assertWalkTo(&walker, 1, {0});
  assertWalkTo(&walker, 2, {0, 1});
  assertWalkTo(&walker, 3, {0, 1});

  assertWalkAll(&walker, {
      {0, 1},
      {0, 2},
      {0, 3},
      {1, 2},
      {1, 3}
  });
}

/*
 * 0 |  x = 1;
 * 1 |  while (x != 1) {
 * 2 |    x = 2;
 *   |  }
 */
CFG getSimpleWhileCFG() {
  CFG cfg("SimpleWhile", 1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 1);
  cfg.addLink(2, CFG_END_NODE);

  return cfg;
}

TEST_CASE("CFG Simple While Static Walk") {
  auto cfg = getSimpleWhileCFG();
  CFGWalker walker(&cfg);

  REQUIRE(walker.walkStatic(0, 1));
  REQUIRE(walker.walkStatic(0, 2));
  REQUIRE(walker.walkStatic(1, 1));
  REQUIRE(walker.walkStatic(1, 2));
  REQUIRE(walker.walkStatic(2, 1));
  REQUIRE(walker.walkStatic(2, 2));

  REQUIRE_FALSE(walker.walkStatic(0, 0));
  REQUIRE_FALSE(walker.walkStatic(1, 0));
  REQUIRE_FALSE(walker.walkStatic(2, 0));
}

TEST_CASE("CFG Simple While Set Walk") {
  auto cfg = getSimpleWhileCFG();
  CFGWalker walker(&cfg);

  assertWalkFrom(&walker, 0, {1, 2});
  assertWalkFrom(&walker, 1, {1, 2});
  assertWalkFrom(&walker, 2, {1, 2});

  assertWalkTo(&walker, 0, {});
  assertWalkTo(&walker, 1, {0, 1, 2});
  assertWalkTo(&walker, 2, {0, 1, 2});

  assertWalkAll(&walker, {
      {0, 1},
      {0, 2},
      {1, 1},
      {1, 2},
      {2, 1},
      {2, 2}
  });
}

/*
 * 0 |  x = 1;
 * 1 |  while (a == b) {
 * 2 |    y = 2;
 * 3 |    if (x == y) then {
 * 4 |      z = 3;
 *   |    } else {
 * 5 |      w = 4;
 * 6 |      a = 5;
 *   |    }
 *   |  }
 */
CFG getSimpleMultiCycleCFG() {
  CFG cfg("SimpleMultiCycle", 1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 3);
  cfg.addLink(3, 4);
  cfg.addLink(4, 2);
  cfg.addLink(4, CFG_END_NODE);

  cfg.addLink(3, 5);
  cfg.addLink(5, 6);
  cfg.addLink(6, 2);
  cfg.addLink(6, CFG_END_NODE);
  return cfg;
}

TEST_CASE("CFG Simple Multi-Cycle Walk") {
  auto cfg = getSimpleMultiCycleCFG();
  CFGWalker walker(&cfg);

  assertWalkFrom(&walker, 0, {1, 2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 1, {2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 2, {2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 3, {2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 4, {2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 5, {2, 3, 4, 5, 6});
  assertWalkFrom(&walker, 6, {2, 3, 4, 5, 6});

  assertWalkTo(&walker, 0, {});
  assertWalkTo(&walker, 1, {0});
  assertWalkTo(&walker, 2, {0, 1, 2, 3, 4, 5, 6});
  assertWalkTo(&walker, 3, {0, 1, 2, 3, 4, 5, 6});
  assertWalkTo(&walker, 4, {0, 1, 2, 3, 4, 5, 6});
  assertWalkTo(&walker, 5, {0, 1, 2, 3, 4, 5, 6});
  assertWalkTo(&walker, 6, {0, 1, 2, 3, 4, 5, 6});

  assertWalkAll(&walker, {
      {0, 1},
      {0, 2},
      {0, 3},
      {0, 4},
      {0, 5},
      {0, 6},

      {1, 2},
      {1, 3},
      {1, 4},
      {1, 5},
      {1, 6},

      {2, 2},
      {2, 3},
      {2, 4},
      {2, 5},
      {2, 6},

      {3, 2},
      {3, 3},
      {3, 4},
      {3, 5},
      {3, 6},

      {4, 2},
      {4, 3},
      {4, 4},
      {4, 5},
      {4, 6},

      {5, 2},
      {5, 3},
      {5, 4},
      {5, 5},
      {5, 6},

      {6, 2},
      {6, 3},
      {6, 4},
      {6, 5},
      {6, 6},
  });
}
