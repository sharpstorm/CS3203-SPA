#include "EntityPredicateFactory.h"

EntityPredicateFactory::EntityPredicateFactory()
    : AbstractPredicateFactory<EntityValue, EntityType>() {}

Predicate<EntityValue> EntityPredicateFactory::handleType(
    IRef<EntityValue, EntityType> *ref) const {
  return predicate;
}

