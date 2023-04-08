#include "ResultProjector.h"
#include "ProjectorWriter.h"

ResultProjector::ResultProjector(const PkbQueryHandler *handler) :
    pkbQueryHandler(handler) {}

void ResultProjector::project(const ProjectableTable *queryResult,
                              const AttributedSynonymList *resultVariables,
                              QPSOutputList *output) {
  // Check if a BOOLEAN type result
  if (resultVariables->empty()) {
    ProjectedValue boolResult = queryResult->isStaticTrue() ? STATIC_TRUE
                                                            : STATIC_FALSE;
    output->push_back(boolResult);
    return;
  }

  // Empty table
  if (!queryResult->hasGroups()) {
    return;
  }

  ProjectorWriter writer(queryResult->getGroups());
  ProjectorIndex projectionIndex = writer
      .buildProjectionIndex(resultVariables, pkbQueryHandler);
  writer.projectTo(output, projectionIndex);
}
