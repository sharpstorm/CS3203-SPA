#pragma once

#include <string>
#include <unordered_set>

#include "IAssignsQueryHandler.h"
#include "ICFGsQueryHandler.h"
#include "ICallsQueryHandler.h"
#include "ICallsTQueryHandler.h"
#include "IDesignEntitiesQueryHandler.h"
#include "IFollowsQueryHandler.h"
#include "IIfPatternQueryHandler.h"
#include "IModifiesQueryHandler.h"
#include "IParentQueryHandler.h"
#include "IUsesQueryHandler.h"
#include "IWhilePatternQueryHandler.h"
#include "IFollowsTQueryHandler.h"
#include "IParentTQueryHandler.h"

class IPkbQueryHandler : public IDesignEntitiesQueryHandler,
                         public IFollowsQueryHandler,
                         public IFollowsTQueryHandler,
                         public IParentQueryHandler,
                         public IParentTQueryHandler,
                         public IModifiesQueryHandler,
                         public IUsesQueryHandler,
                         public ICallsQueryHandler,
                         public ICallsTQueryHandler,
                         public IIfPatternQueryHandler,
                         public IWhilePatternQueryHandler,
                         public IAssignsQueryHandler,
                         public ICFGsQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
};
