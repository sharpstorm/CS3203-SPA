#pragma once

#include <unordered_set>

#include "common/Types.h"

template <class ClosureType>
using StmtTypePredicate = bool(*)(const ClosureType &state,
                                  StmtType type, StmtValue value);

template <class ClosureType>
using ModifiesGetter = EntityValue(*)(const ClosureType &state,
                                      StmtValue value);

template <class ClosureType>
using UsesGetter = unordered_set<EntityValue>(*)(const ClosureType &state,
                                                 StmtValue value);
