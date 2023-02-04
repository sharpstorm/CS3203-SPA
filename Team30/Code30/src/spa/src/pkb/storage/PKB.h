#pragma once

#include "../predicates/PredicateFactory.h"
//#include "FollowsStorage.h"
#include "ParentStorage.h"
#include "StorageTypes.h"
#include "StructureMappingProvider.h"

class PKB {
 public:
  PKB();

  FollowsStorage* followsStore;
  ParentStorage* parentStore;
  StructureMappingProvider* structureProvider;
  PredicateFactory* predicateFactory;
};
