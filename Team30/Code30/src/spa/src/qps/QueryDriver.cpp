#include <memory>

#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  unique_ptr<PQLQuery> pqlQuery = parser->parseQuery(query);
  PQLQueryResult* pqlResult = executor->executeQuery(pqlQuery.get());
  PQLQuerySynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector.project(pqlResult, queryVar);
  delete(pqlResult);
  return result;
}

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH):
  parser(new QueryParser()), executor(new QueryExecutor(pkbQH)) {
}

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}
