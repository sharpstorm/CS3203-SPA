#pragma once

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "AbstractPredicateFactory.h"

class EntityPredicateFactory : public AbstractPredicateFactory<EntityValue,
                                                               EntityType> {
 public:
  EntityPredicateFactory();

  Predicate<EntityValue> handleType(
      IRef<EntityValue, EntityType> *ref) const override;

 private:
  Predicate<EntityValue> predicate = [](EntityValue const s) { return true; };
};
