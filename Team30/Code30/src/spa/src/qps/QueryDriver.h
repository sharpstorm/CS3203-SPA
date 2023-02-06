#pragma once

#include <string>
#include <memory>
#include "IQueryDriver.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"

using std::string;

class QueryDriver: public IQueryDriver {
 private:
  IQueryParser* parser;
  IQueryExecutor* executor;
 public:
  explicit QueryDriver(shared_ptr<PkbQueryHandler> pkbQH);
  QueryDriver(IQueryParser* parser, IQueryExecutor* executor);
  ~QueryDriver();
  PQLQueryResult* evaluate(string* query);
};
