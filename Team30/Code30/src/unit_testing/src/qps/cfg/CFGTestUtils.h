#pragma once

#include "common/Types.h"

class CFGTestUtils {
 public:
  static bool dummyTypePredicate(const int &state, StmtType type,
                                 StmtValue value) {
    return true;
  }
};
