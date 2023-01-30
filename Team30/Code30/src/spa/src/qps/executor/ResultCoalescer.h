#pragma once

#include "../common/IntermediateResultSet.h"

class ResultCoalescer {
 public:
  IntermediateResultSet* merge(IntermediateResultSet* setA,
                               IntermediateResultSet* setB);
};
