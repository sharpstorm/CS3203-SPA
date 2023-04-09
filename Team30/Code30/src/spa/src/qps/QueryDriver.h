#pragma once

#include "qps/executor/IQueryExecutor.h"
#include "qps/parser/IQueryParser.h"
#include "qps/projector/ResultProjector.h"
#include "common/parser/ISourceExpressionParser.h"
#include "qps/common/QPSTypes.h"
#include "IQPS.h"

class QueryDriver : public IQPS {
 private:
  IQueryParser *parser;
  IQueryExecutor *executor;
  IProjector *projector;
 public:
  explicit QueryDriver(const PkbQueryHandler *pkbQH,
                       ISourceExpressionParser *exprParser);
  ~QueryDriver();
  void evaluate(const PQLQueryString &query,
                QPSOutputList *output) override;
};
