#include <vector>
#include <memory>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(
    PQLQueryResult *queryResult,
    PQLQuerySynonymList* resultVariables
) {
  UniqueVectorPtr<string> result =
      make_unique<vector<string>>(vector<string>{});
  // Static result
  if (queryResult->isFalse()) {
    return result;
  }

  // TODO(kwanhw): fix projection tupling
  for (auto it = resultVariables->begin();
       it != resultVariables->end(); it++) {
    PQLSynonymName synName = it->getName();
    ResultTableCol col = queryResult->getSynonymCol(synName);

    if (col != PQLQueryResult::NO_COL) {
      for (int i = 0; i < queryResult->getRowCount(); i++) {
        result->push_back(
            queryResult
                ->getTableRowAt(i)
                ->at(col)
                ->project());
      }
    }
  }

  return result;
}
