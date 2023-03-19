#pragma once

#include <memory>
#include "qps/common/PQLTypes.h"
#include "OverrideTransformer.h"

using std::unique_ptr;

typedef unordered_map<PQLSynonymName, OverrideTransformer> OverrideTable;
typedef unique_ptr<OverrideTable> OverrideTablePtr;
