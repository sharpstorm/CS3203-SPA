#pragma once

#include "common/Types.h"

template<class ClosureType>
using StmtTypePredicate = bool (*)(const ClosureType &state,
                                   StmtType type, StmtValue value);

template<class ClosureType>
using ModifiesGetter = EntityIdxSet(*)(const ClosureType &state,
                                       StmtValue value);

template<class ClosureType>
using UsesGetter = EntityIdxSet(*)(const ClosureType &state,
                                   StmtValue value);
