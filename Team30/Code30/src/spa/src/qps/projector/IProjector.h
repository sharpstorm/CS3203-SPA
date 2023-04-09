#pragma once

#include "qps/common/projector_table/ProjectableTable.h"

class IProjector {
 public:
  virtual ~IProjector() = default;
  virtual void project(const ProjectableTable *queryResult,
                       const AttributedSynonymList *resultVariables,
                       QPSOutputList *output) = 0;
};