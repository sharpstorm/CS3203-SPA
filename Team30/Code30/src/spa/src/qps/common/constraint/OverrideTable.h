#pragma once

#include <memory>
#include <unordered_map>

#include "qps/common/PQLTypes.h"
#include "OverrideTransformer.h"

using std::unique_ptr, std::unordered_map;

typedef unordered_map<PQLSynonymName, OverrideTransformer> OverrideTable;
typedef unique_ptr<OverrideTable> OverrideTablePtr;
