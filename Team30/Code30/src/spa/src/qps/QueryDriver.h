#pragma once

#include <string>

#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"
#include "qps/projector/ResultProjector.h"
#include "common/parser/ISourceExpressionParser.h"
#include "IQPS.h"

using std::string;

class QueryDriver: public IQPS {
 private:
  IQueryParser* parser;
  IQueryExecutor* executor;
  ResultProjector* projector;
 public:
  explicit QueryDriver(PkbQueryHandler* pkbQH,
                       ISourceExpressionParser* exprParser);
  ~QueryDriver();
  UniqueVectorPtr<string> evaluate(const string &query) override;
};
