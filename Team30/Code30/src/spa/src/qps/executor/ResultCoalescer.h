#pragma once

#include <string>
#include <unordered_set>
#include "qps/common/PQLQueryResult.h"

using std::string, std::unordered_set;

class ResultCoalescer {
 public:
  PQLQueryResult* merge(PQLQueryResult* setA,
                        PQLQueryResult* setB);

 private:
  bool mergeStaticResult(PQLQueryResult *setA,
                         PQLQueryResult *setB);
  string mergeError(PQLQueryResult *setA, PQLQueryResult *setB);
  void mergeResult(PQLQueryResult *setA,
                         PQLQueryResult *setB,
                         PQLQueryResult* output);

  template<class T>
  unordered_set<T>* intersectSet(unordered_set<T>* s1, unordered_set<T>* s2);
};
