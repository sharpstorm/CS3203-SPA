#include "QPSFacade.h"

QPSFacade::QPSFacade(IQueryDriver driver) : driver(driver) {
}

string *QPSFacade::evaluate(string query) {
    PQLQueryResult* queryResult = driver.evaluate(&query);
    string* projectedResult = projector.project(queryResult);
    return projectedResult;
}
