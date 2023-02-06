#include <memory>

#include "QueryDriver.h"

PQLQueryResult *QueryDriver::evaluate(string* query) {
  unique_ptr<PQLQuery> pqlQuery = parser->parseQuery(query);
  PQLQueryResult* result = executor->executeQuery(pqlQuery.get());
  return result;
}

QueryDriver::QueryDriver(IQueryParser* parser, IQueryExecutor* executor) :
  parser(parser), executor(executor) {
}
