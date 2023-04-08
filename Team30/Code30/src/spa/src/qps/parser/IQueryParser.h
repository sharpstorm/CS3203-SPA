#pragma once

#include "../common/PQLQuery.h"
#include "qps/common/QPSTypes.h"

class IQueryParser {
 public:
  virtual ~IQueryParser() = default;
  virtual PQLQueryPtr parseQuery(const PQLQueryString *query) = 0;
};
