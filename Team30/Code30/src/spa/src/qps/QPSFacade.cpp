#include "QPSFacade.h"

string *QPSFacade::evaluate(string query) {
    PQLQueryResult* queryResult = driver.evaluate(&query);
    string* projectedResult = projector.project(queryResult);
    return projectedResult;
}
