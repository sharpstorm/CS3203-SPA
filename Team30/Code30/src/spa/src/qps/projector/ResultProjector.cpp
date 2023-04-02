#include <vector>
#include <memory>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

ResultProjector::ResultProjector(PkbQueryHandler *handler):
    pkbQueryHandler(handler) { }

void ResultProjector::project(ProjectorResultTable *queryResult,
                              AttributedSynonymList*resultVariables,
                              QPSOutputList *output) {
  // Check if a BOOLEAN type result
  if (queryResult->getIsBooleanResult()) {
    string boolResult = queryResult->getBooleanResult() ? "TRUE" : "FALSE";
    output->push_back(boolResult);
    return;
  }

  int groupCount = queryResult->getResultGroupCount();
  // Empty table
  if (groupCount == 0) {
    return;
  }

  // Cross product
  ProjectorResultGroup* finalGroup = queryResult->getResultGroup(0);
  for (int i=1; i < queryResult->getResultGroupCount(); i++) {
    // TODO(KwanHW): So far no mem leaks, look here if there is
    finalGroup = finalGroup->crossProduct(queryResult->getResultGroup(i));
  }

  // Map and project to tuple
  finalGroup->project(resultVariables, pkbQueryHandler, output);
}
