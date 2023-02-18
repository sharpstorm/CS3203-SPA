#pragma once

#include <string>
#include <unordered_set>
#include "qps/common/PQLQueryResult.h"

using std::string, std::unordered_set;

template<class T>
using IntersectSet = unordered_set<T>;

template<class T>
using IntersectSetPtr = unique_ptr<IntersectSet<T>>;

class ResultCoalescer {
 public:
  PQLQueryResult* merge(PQLQueryResult* setA,
                        PQLQueryResult* setB);

 private:
  struct IntersectState {
    vector<ResultTableCol> leftCommons;
    vector<ResultTableCol> rightCommons;
    vector<ResultTableCol> rightColsToCopy;
    vector<ResultTableCol> leftColsToCopy;
  };

  struct IntersectResult {
    IntersectSetPtr<int> leftSet;
    IntersectSetPtr<int> rightSet;
  };

  struct InternalMergeState {
    PQLQueryResult *setA;
    PQLQueryResult *setB;
    PQLQueryResult* output;
  };

  void mergeResult(InternalMergeState* state);
  void mergeStaticResult(InternalMergeState* state);
  void mergeError(InternalMergeState* state);


  void mergeSynonymList(InternalMergeState* mergeState,
                        IntersectState *intersectState);

  IntersectResult findIntersect(InternalMergeState* mergeState,
                                QueryResultTableRow* currentRow,
                                IntersectState* state);

  void crossProduct(InternalMergeState* mergeState,
                    unordered_set<int>* ignoreSet,
                    IntersectState* intersectState,
                    IntersectResult* intersection);

  void mergeRow(QueryResultTableRow* rowA,
                QueryResultTableRow* rowB,
                QueryResultTableRow* outputRow,
                IntersectState* state);

  template<class T>
  IntersectSetPtr<T> intersectSet(IntersectSet<T>* s1,
                                  IntersectSet<T>* s2);
};
