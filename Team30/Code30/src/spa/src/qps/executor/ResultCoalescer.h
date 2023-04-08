#pragma once

#include <set>
#include <memory>
#include <vector>

#include "common/UtilityTypes.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::set, std::unique_ptr, std::vector;

class ResultCoalescer {
  typedef vector<ResultTableCol> ResultTableColList;
  typedef set<ResultTableRow> ResultTableRowSet;
 private:
  struct IntersectResult {
    RowSetPtr leftSet;
    RowSetPtr rightSet;

    bool isEmpty() const;
  };

  PQLQueryResult *setA;
  PQLQueryResult *setB;
  PQLQueryResult *output;

  OrphanedResultItemPoolPtr orphanMap;

  ResultTableColList leftCommons;
  ResultTableColList rightCommons;
  ResultTableColList rightColsToCopy;
  ResultTableColList leftColsToCopy;

 public:
  ResultCoalescer(PQLQueryResult *setA, PQLQueryResult *setB);
  PQLQueryResult *merge();

 private:
  void mergeResults();
  void mergeSynonymList();

  IntersectResult findIntersect(
      const QueryResultTableRow *currentRow) const;

  void crossProduct(ResultTableRowSet *ignoreSet,
                    const IntersectResult *intersection);

  void mergeRow(const QueryResultTableRow *rowA,
                const QueryResultTableRow *rowB,
                QueryResultTableRow *outputRow) const;
  void swapSets();
};
