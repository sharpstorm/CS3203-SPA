#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "qps/common/synonym/PQLQuerySynonym.h"

QueryDriver::QueryDriver(const PkbQueryHandler *pkbQH,
                         ISourceExpressionParser *exprParser) :
    parser(new QueryParser(exprParser)),
    executor(new QueryExecutor(pkbQH)),
    projector(new ResultProjector(pkbQH)) {}

QueryDriver::~QueryDriver() {
  delete (parser);
  delete (executor);
  delete (projector);
}

void QueryDriver::evaluate(const PQLQueryString &query,
                           QPSOutputList *output) {
  PQLQueryPtr pqlQuery = parser->parseQuery(&query);
  ProjectableTable *synTable = executor->executeQuery(pqlQuery.get());
  const AttributedSynonymList *queryVar = pqlQuery->getResultVariables();
  projector->project(synTable, queryVar, output);
  delete (synTable);
}
