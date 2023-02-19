#pragma once

#include <string>
#include <memory>
#include "IQueryDriver.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"
#include "qps/projector/ResultProjector.h"

using std::string;

class QueryDriver: public IQueryDriver {
 private:
  IQueryParser* parser;
  IQueryExecutor* executor;
  ResultProjector projector;
 public:
  explicit QueryDriver(shared_ptr<PkbQueryHandler> pkbQH);
  ~QueryDriver();
  UniqueVectorPtr<string> evaluate(string* query);
};
