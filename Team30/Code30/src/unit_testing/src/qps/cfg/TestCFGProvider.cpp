#include "TestCFGProvider.h"

/*
 * 0 |  x = 1;
 * 1 |  y = 2;
 * 2 |  z = x;
 * 3 |  w = y;
 */
CFG TestCFGProvider::getLinearCFG() {
  CFG cfg(1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 3);
  cfg.addLink(3, CFG_END_NODE);

  return cfg;
}

/*
 * 0 |  x = 1;
 * 1 |  if (x != 1) then {
 * 2 |    x = 2;
 *   |  } else {
 * 3 |    x = 3;
 *   |  }
 */
CFG TestCFGProvider::getSimpleIfCFG() {
  CFG cfg(1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(1, 3);
  cfg.addLink(2, CFG_END_NODE);
  cfg.addLink(3, CFG_END_NODE);

  return cfg;
}

/*
 * 0 |  x = 1;
 * 1 |  while (x != 1) {
 * 2 |    x = 2;
 *   |  }
 */
CFG TestCFGProvider::getSimpleWhileCFG() {
  CFG cfg(1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 1);
  cfg.addLink(2, CFG_END_NODE);

  return cfg;
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
CFG TestCFGProvider::getSimpleMultiCycleCFG() {
  CFG cfg(1);

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

/*
 * 0 |  x = 1;
 * 1 |  if (x != 1) then {
 * 2 |    x = x;
 *   |  } else {
 * 3 |    x = 2;
 *   |  }
 * 4 |  x = x;
 */
CFG TestCFGProvider::getAffectsIfCFG() {
  CFG cfg(1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(1, 3);
  cfg.addLink(2, 4);
  cfg.addLink(3, 4);
  cfg.addLink(4, CFG_END_NODE);

  return cfg;
}

/*
 * 0 |  x = 1;
 * 1 |  while (x != 1) then {
 * 2 |    x = x;
 *   |  }
 * 3 |  x = x;
 */
CFG TestCFGProvider::getAffectsWhileCFG() {
  CFG cfg(1);

  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(1, 3);
  cfg.addLink(2, 3);
  cfg.addLink(2, 1);
  cfg.addLink(3, CFG_END_NODE);

  return cfg;
}

