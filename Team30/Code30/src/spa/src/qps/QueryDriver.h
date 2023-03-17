#pragma once

#include <string>

#include "IQueryDriver.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"
#include "qps/projector/ResultProjector.h"
#include "common/parser/ISourceExpressionParser.h"

using std::string;

class QueryDriver: public IQueryDriver {
 private:
  IQueryParser* parser;
  IQueryExecutor* executor;
  ResultProjector projector;
  PkbQueryHandler* pkbQueryHandler;
 public:
  explicit QueryDriver(PkbQueryHandler* pkbQH,
                       ISourceExpressionParser* exprParser);
  ~QueryDriver();
  UniqueVectorPtr<string> evaluate(string* query);
};
