#pragma once

#include "common/cfg/CFG.h"
#include "qps/cfg/CFGQuerierTypes.h"

class TestCFGProvider {
 public:
  static CFG getLinearCFG();
  static CFG getSimpleIfCFG();
  static CFG getSimpleWhileCFG();
  static CFG getSimpleMultiCycleCFG();
  static CFG getAffectsIfCFG();
  static CFG getAffectsWhileCFG();
  static CFG getLinearTransitiveCFG();
  static CFG getAffectsTWhileCFG();
  static CFG getAffectsTIfCFG();
};
