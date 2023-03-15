#pragma once

#include "common/cfg/CFG.h"

class TestCFGProvider {
 public:
  static CFG getLinearCFG();
  static CFG getSimpleIfCFG();
  static CFG getSimpleWhileCFG();
  static CFG getSimpleMultiCycleCFG();
};
