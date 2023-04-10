#pragma once

#include "common/Types.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class ClosureType>
using StmtTypePredicate = bool (*)(const ClosureType &state,
                                   StmtType type, StmtValue value);
