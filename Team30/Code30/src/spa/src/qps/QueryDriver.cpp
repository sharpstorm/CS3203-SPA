#include "QueryDriver.h"

QueryResult *QueryDriver::evaluate(string* query) {
    PQLQuery* pqlQuery = parser.parseQuery(query);
    // QueryResult* result = executor.executeQuery(query);
    // return result;
  return nullptr;
}
