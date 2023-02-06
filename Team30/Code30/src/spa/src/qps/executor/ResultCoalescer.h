#pragma once

#include "qps/common/PQLQueryResult.h"

class ResultCoalescer {
 public:
  PQLQueryResult* merge(PQLQueryResult* setA,
                        PQLQueryResult* setB);
};
