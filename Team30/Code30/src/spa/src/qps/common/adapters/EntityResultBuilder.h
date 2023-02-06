#pragma once

#include <unordered_set>
#include <utility>

#include "common/Types.h"
#include "qps/common/PQLQueryResult.h"

using std::pair, std::unordered_set;

class EntityResultBuilder {
 public:
  static EntityResult buildEntityResult(bool isLeft,
                                              QueryResult<int, string>
                                                  queryResult);
  static EntityResult buildEntityResult(unordered_set<string> items);
};
