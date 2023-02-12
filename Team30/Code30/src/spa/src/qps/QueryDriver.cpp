#include <memory>

#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  unique_ptr<PQLQuery> pqlQuery = parser->parseQuery(query);
  PQLQueryResult* pqlResult = executor->executeQuery(pqlQuery.get());
  UniqueVectorPtr<string> result = projector.project(pqlResult);
  return result;
}

QueryDriver::QueryDriver(shared_ptr<PkbQueryHandler> pkbQH):
  parser(new QueryParser()), executor(new QueryExecutor(pkbQH)) {
}

QueryDriver::QueryDriver(IQueryParser* parser, IQueryExecutor* executor) :
  parser(parser), executor(executor) {
}

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}
