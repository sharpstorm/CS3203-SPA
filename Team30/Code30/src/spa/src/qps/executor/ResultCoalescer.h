#pragma once

#include <set>
#include <memory>
#include <vector>

#include "common/UtilityTypes.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::set, std::unique_ptr, std::vector;

class ResultCoalescer {
 private:
  PQLQueryResult *setA;
  PQLQueryResult *setB;
  PQLQueryResult *output;

  OrphanedResultItemPoolPtr orphanMap;

 public:
  ResultCoalescer(PQLQueryResult *setA, PQLQueryResult *setB);
  PQLQueryResult *merge();

 private:
  struct IntersectState {
    vector<ResultTableCol> leftCommons;
    vector<ResultTableCol> rightCommons;
    vector<ResultTableCol> rightColsToCopy;
    vector<ResultTableCol> leftColsToCopy;
  };

  struct IntersectResult {
    RowSetPtr leftSet;
    RowSetPtr rightSet;

    bool isEmpty() const {
      return leftSet->empty() || rightSet->empty();
    }
  };

  void mergeResults();
  void mergeSynonymList(IntersectState *intersectState);

  IntersectResult findIntersect(const QueryResultTableRow *currentRow,
                                const IntersectState *state) const;

  void crossProduct(set<ResultTableRow> *ignoreSet,
                    const IntersectState *intersectState,
                    const IntersectResult *intersection);

  void mergeRow(const QueryResultTableRow *rowA,
                const QueryResultTableRow *rowB,
                QueryResultTableRow *outputRow,
                const IntersectState *state) const;
};
