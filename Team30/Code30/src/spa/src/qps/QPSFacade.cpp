#include "QPSFacade.h"

string *QPSFacade::evaluate(string query) {
    QueryResult* queryResult = driver.evaluate(&query);
    string* projectedResult = projector.project(queryResult);
    // return projectedResult;
    return nullptr;
}
