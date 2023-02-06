#include <vector>

#include "QPSFacade.h"

using std::vector;

QPSFacade::QPSFacade(IQueryDriver* driver) : driver(driver) {
}

vector<string> *QPSFacade::evaluate(string query) {
    PQLQueryResult* queryResult = driver->evaluate(&query);
    vector<string>* projectedResult = projector.project(queryResult);
    return projectedResult;
}
