#include <vector>
#include <memory>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

ResultProjector::ResultProjector(const PkbQueryHandler *handler) :
    pkbQueryHandler(handler) {}

void ResultProjector::project(const ProjectorResultTable *queryResult,
                              const AttributedSynonymList *resultVariables,
                              QPSOutputList *output) {
  // Check if a BOOLEAN type result
  if (queryResult->getIsBooleanResult()) {
    string boolResult = queryResult->getBooleanResult()
                        ? STATIC_TRUE : STATIC_FALSE;
    output->push_back(boolResult);
    return;
  }

  int groupCount = queryResult->getResultGroupCount();
  // Empty table
  if (groupCount == 0) {
    return;
  }

  ProjectorIndex projectionIndex = queryResult
      ->buildProjectionIndex(resultVariables, pkbQueryHandler);
  queryResult->projectTo(output, projectionIndex);
}
