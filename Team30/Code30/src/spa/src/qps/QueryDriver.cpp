#include "QueryDriver.h"
#include <memory>

QueryResult *QueryDriver::evaluate(string* query) {
  unique_ptr<PQLQuery> pqlQuery = parser->parseQuery(query);
  QueryResult* result = executor.executeQuery(pqlQuery.get());
  return result;
}
