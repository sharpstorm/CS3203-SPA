#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH,
                         ISourceExpressionParser* exprParser):
    parser(new QueryParser(exprParser)),
    executor(new QueryExecutor(pkbQH)),
    projector(new ResultProjector(pkbQH)) { }

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
  delete(projector);
}

UniqueVectorPtr<string> QueryDriver::evaluate(const string &query) {
  PQLQueryPtr pqlQuery = parser->parseQuery(&query);
  SynonymResultTable* synTable = executor->executeQuery(pqlQuery.get());
  AttributedSynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector->project(
      synTable, queryVar);
  delete(synTable);
  return result;
}

