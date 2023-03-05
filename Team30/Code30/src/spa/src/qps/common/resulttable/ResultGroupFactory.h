#pragma once

#include <memory>
#include <vector>

#include "ResultGroup.h"
#include "common/UtilityTypes.h"
#include "qps/common/PQLQueryResult.h"

using std::unique_ptr, std::vector;

class ResultGroupFactory {
  static IntersectSet<int> getUniqueRows(PQLQueryResult* result,
                                  vector<PQLSynonymName>* syns);
 public:
  static unique_ptr<ResultGroup> extractResults(PQLQueryResult* result,
                                                vector<PQLSynonymName>* syns);
};
