#include "PKB.h"

PKB::PKB()
    : followsStore(),
      parentStore(),
      structureProvider(),
      predicateFactory(new PredicateFactory(structureProvider)) {}
