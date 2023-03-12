#pragma once

#include "common/Types.h"

template <class ClosureType>
using StmtTypePredicate = bool(*)(ClosureType* state,
                                  StmtType type, StmtValue value);
