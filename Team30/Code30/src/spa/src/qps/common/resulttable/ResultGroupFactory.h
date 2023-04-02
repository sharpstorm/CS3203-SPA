#pragma once

#include <memory>
#include <vector>

#include "ResultGroup.h"
#include "common/UtilityTypes.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::unique_ptr, std::vector;

class ResultGroupFactory {
  static IntersectSet<int> getUniqueRows(PQLQueryResult* result,
                                  vector<PQLSynonymName>* syns);
 public:
  static ResultGroupPtr extractResults(PQLQueryResult* result,
                                                vector<PQLSynonymName>* syns);
};
