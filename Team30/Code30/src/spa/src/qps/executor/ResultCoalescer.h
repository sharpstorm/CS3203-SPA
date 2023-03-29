#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>

#include "common/UtilityTypes.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::string, std::set, std::unique_ptr, std::vector;

class ResultCoalescer {
 private:
  PQLQueryResult *setA;
  PQLQueryResult *setB;
  PQLQueryResult *output;

  OrphanedResultItemPoolPtr orphanMap;


 public:
  ResultCoalescer(PQLQueryResult *setA, PQLQueryResult *setB);
  PQLQueryResult* merge();

 private:
  struct IntersectState {
    vector<ResultTableCol> leftCommons;
    vector<ResultTableCol> rightCommons;
    vector<ResultTableCol> rightColsToCopy;
    vector<ResultTableCol> leftColsToCopy;
  };

  struct IntersectResult {
    BSTIntersectSetPtr<ResultTableRow> leftSet;
    BSTIntersectSetPtr<ResultTableRow> rightSet;

    bool isEmpty() {
      return leftSet->empty() || rightSet->empty();
    }
  };

  void mergeResults();
  void mergeSynonymList(IntersectState *intersectState);

  IntersectResult findIntersect(QueryResultTableRow *currentRow,
                                IntersectState *state);

  void crossProduct(set<ResultTableRow> *ignoreSet,
                    IntersectState *intersectState,
                    IntersectResult *intersection);

  void mergeRow(QueryResultTableRow *rowA,
                QueryResultTableRow *rowB,
                QueryResultTableRow *outputRow,
                IntersectState *state);
};
