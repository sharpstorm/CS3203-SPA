#pragma once

#include <string>
#include <unordered_set>

#include "IAssignQueryHandler.h"
#include "IDesignEntitiesQueryHandler.h"
#include "IFollowsQueryHandler.h"
#include "IParentQueryHandler.h"

using std::string;
using std::unordered_set;

class IPkbQueryHandler : public IFollowsQueryHandler,
                         public IParentQueryHandler,
                         public IDesignEntitiesQueryHandler,
                         public IAssignQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
};
