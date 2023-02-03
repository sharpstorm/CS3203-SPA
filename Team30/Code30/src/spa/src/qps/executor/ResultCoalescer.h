#pragma once

#include "../common/QueryResult.h"

class ResultCoalescer {
 public:
  QueryResult* merge(QueryResult* setA,
                               QueryResult* setB);
};
