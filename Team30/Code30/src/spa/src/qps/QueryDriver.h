#pragma once

#include <string>
#include "IQueryDriver.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"

using std::string;

class QueryDriver: public IQueryDriver {
    IQueryParser parser;
    IQueryExecutor executor;
 public:
  PQLQueryResult* evaluate(string* query);
};
