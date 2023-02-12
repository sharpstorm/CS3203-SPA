#pragma once

#include <string>
#include <unordered_set>

#include "IFollowsQueryHandler.h"
#include "IParentQueryHandler.h"
#include "IDesignEntitiesQueryHandler.h"
#include "IUsesQueryHandler.h"
#include "IModifiesQueryHandler.h"

class IPkbQueryHandler
    : public IFollowsQueryHandler,
      public IParentQueryHandler,
      public IDesignEntitiesQueryHandler,
      public IUsesQueryHandler,
      public IModifiesQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
};
