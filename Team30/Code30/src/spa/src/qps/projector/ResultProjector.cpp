#include <vector>
#include <memory>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(PQLQueryResult *queryResult,
                                                 PQLQuerySynonym resultVariable
) {
  UniqueVectorPtr<string> result =
      make_unique<vector<string>>(vector<string>{});
  // Error result
  if (!queryResult->getError().empty()) {
    result->push_back(queryResult->getError());
    return result;
  }

  // Static result
  if (queryResult->isFalse()) {
    return result;
  }

  PQLSynonymName var = resultVariable.getName();
  ResultTableCol col = queryResult->getSynonymCol(var);
  if (col != PQLQueryResult::NO_COL) {
    for (int i = 0; i < queryResult->getRowCount(); i++) {
      result->push_back(
          queryResult
              ->getTableRowAt(i)
              ->at(col)
              ->project());
    }
  }

  return result;
}
