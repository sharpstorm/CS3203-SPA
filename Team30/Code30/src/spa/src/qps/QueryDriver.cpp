#include "QueryDriver.h"

PQLQueryResult *QueryDriver::evaluate(string* query) {
    PQLQuery* pqlQuery = parser.parseQuery(query);
    // PQLQueryResult* result = executor.executeQuery(query);
    // return result;
  return nullptr;
}
