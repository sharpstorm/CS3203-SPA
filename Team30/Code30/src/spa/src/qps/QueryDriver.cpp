#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  PQLQueryPtr pqlQuery = parser->parseQuery(query);
  PQLQueryResult* pqlResult = executor->executeQuery(pqlQuery.get());
  PQLQuerySynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector.project(pqlResult, queryVar);
  delete(pqlResult);
  return result;
}

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH,
                         ISourceExpressionParser* exprParser):
  parser(new QueryParser(exprParser)), executor(new QueryExecutor(pkbQH)) {
}

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}
