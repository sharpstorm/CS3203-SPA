#pragma once

#include <unordered_set>

#include "common/Types.h"

template <class ClosureType>
using StmtTypePredicate = bool(*)(ClosureType* state,
                                  StmtType type, StmtValue value);

template <class ClosureType>
using ModifiesGetter = EntityValue(*)(ClosureType* state, StmtValue value);

template <class ClosureType>
using UsesGetter = unordered_set<EntityValue>(*)(ClosureType* state,
                                                 StmtValue value);
