#include <vector>
#include <memory>
#include <iostream>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(
    SynonymResultTable *queryResult, PQLQuerySynonymList *resultVariables) {
  UniqueVectorPtr<string> result =
      make_unique<vector<string>>(vector<string>{});

  // Check if a BOOLEAN type result
  if (queryResult->getIsBooleanResult()) {
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
    // TODO(KwanHW): So far no mem leaks, look here if there is
    finalGroup = finalGroup->crossProduct(queryResult->getResultGroup(i));
  }

  // Map and project to tuple
  finalGroup->project(resultVariables, result.get());
  return result;
}
