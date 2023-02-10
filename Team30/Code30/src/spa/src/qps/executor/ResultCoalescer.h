#pragma once

#include <string>
#include "qps/common/PQLQueryResult.h"

using std::string;

class ResultCoalescer {
 public:
  PQLQueryResult* merge(PQLQueryResult* setA,
                        PQLQueryResult* setB);

 private:
  bool mergeStaticResult(PQLQueryResult *setA,
                         PQLQueryResult *setB);
  string mergeError(PQLQueryResult *setA, PQLQueryResult *setB);
  void mergeEntityResult(PQLQueryResult *setA,
                         PQLQueryResult *setB,
                         PQLQueryResult* output);
  void mergeStatementResult(PQLQueryResult *setA,
                            PQLQueryResult *setB,
                         PQLQueryResult* output);
};
