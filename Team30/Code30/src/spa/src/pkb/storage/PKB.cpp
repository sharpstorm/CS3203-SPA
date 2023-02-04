#include "PKB.h"

#include <memory>

#include "tables/ContiguousTable.h"
using std::make_shared;

PKB::PKB()
    : followsStore(new FollowsStorage(make_shared<ContiguousTable<int>>(),
                                      make_shared<ContiguousTable<int>>())),
      parentStore(),
      structureProvider(),
      predicateFactory(new PredicateFactory(structureProvider)) {}
