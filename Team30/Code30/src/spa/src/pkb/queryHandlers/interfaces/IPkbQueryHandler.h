#pragma once

#include <string>
#include <unordered_set>

#include "IFollowsQueryHandler.h"
#include "IParentQueryHandler.h"
#include "IDesignEntitiesQueryHandler.h"

using std::unordered_set;
using std::string;

class IPkbQueryHandler
    : public IFollowsQueryHandler,
      public IParentQueryHandler,
      public IDesignEntitiesQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
};
