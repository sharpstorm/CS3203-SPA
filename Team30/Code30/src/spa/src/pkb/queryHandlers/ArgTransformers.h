#pragma once

#include "pkb/PkbTypes.h"
#include "common/Types.h"

using pkb::ArgTransformer;

constexpr pkb::ArgTransformer<EntityValue, EntityType> procArgTransformer =
    [](IRef<EntityValue, EntityType> *arg) {
      arg->setType(EntityType::Procedure);
      return arg;
    };
