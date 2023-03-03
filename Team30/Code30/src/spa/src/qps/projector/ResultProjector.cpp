#include <vector>
#include <memory>
#include <iostream>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

// ! OLD IMPLEMENTATION
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

UniqueVectorPtr<string> ResultProjector::project(SynonymResultTable *queryResult,
                                                 PQLQuerySynonymList *resultVariables) {
  UniqueVectorPtr<string> result =
      make_unique<vector<string>>(vector<string>{});

  // Check if a BOOLEAN type result
  if (!queryResult->hasTargetSynonyms()) {
    string boolResult = queryResult->getBooleanResult() ? "TRUE" : "FALSE";
    return make_unique<vector<string>>(vector<string>({boolResult}));
  }

  int groupCount = queryResult->getResultGroupCount();
  // Empty table
  if (groupCount == 0) {
    return result;
  }

  // Cross product
  ResultGroup* finalGroup = queryResult->getResultGroup(0);
  for (int i=1; i < queryResult->getResultGroupCount(); i++) {
    // TODO Probably will have a mem leak here
    finalGroup = finalGroup->crossProduct(queryResult->getResultGroup(i));
  }

  // Map and project to tuple
  finalGroup->project(resultVariables, result.get());
  return result;
}
