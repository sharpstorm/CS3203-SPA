#include "QueryDriver.h"

PQLQueryResult *QueryDriver::evaluate(string* query) {
//     shared_ptr<PQLQuery> pqlQuery = parser.parseQuery(query);
//     PQLQueryResult* result = executor.executeQuery(query);
    // return result;
  return nullptr;
}

QueryDriver::QueryDriver(IQueryParser parser, IQueryExecutor executor) :
  parser(parser), executor(executor){
}
