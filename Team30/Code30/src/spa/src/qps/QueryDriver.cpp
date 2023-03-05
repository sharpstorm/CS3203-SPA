#include <iostream>
#include <memory>

#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  unique_ptr<PQLQuery> pqlQuery = parser->parseQuery(query);
  SynonymResultTable* synTable = executor->executeQuery(pqlQuery.get());
  PQLQuerySynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector.project(synTable, queryVar);
  delete(synTable);
  return result;
}

QueryDriver::QueryDriver(shared_ptr<PkbQueryHandler> pkbQH):
  parser(new QueryParser()), executor(new QueryExecutor(pkbQH)) {
}

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}
