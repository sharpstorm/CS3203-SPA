#pragma once

#include <string>
#include <unordered_set>

#include "IAssignQueryHandler.h"
#include "IDesignEntitiesQueryHandler.h"
#include "IFollowsQueryHandler.h"
#include "IModifiesQueryHandler.h"
#include "IParentQueryHandler.h"
#include "IUsesQueryHandler.h"

class IPkbQueryHandler : public IFollowsQueryHandler,
                         public IParentQueryHandler,
                         public IDesignEntitiesQueryHandler,
                         public IUsesQueryHandler,
                         public IModifiesQueryHandler,
                         public IAssignQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
};
