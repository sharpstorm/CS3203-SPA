#pragma once

#include "common/Types.h"

class CFGTestUtils {
 public:
  static bool dummyTypePredicate(int* state, StmtType type, StmtValue value) {
    return true;
  }
};
