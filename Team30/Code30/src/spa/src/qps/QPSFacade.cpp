#include <vector>

#include "QPSFacade.h"

using std::vector;

QPSFacade::QPSFacade(shared_ptr<PkbQueryHandler> pkbQH) {
  driver = new QueryDriver(pkbQH);
}

QPSFacade::~QPSFacade() noexcept {
  delete(driver);
}

vector<string> *QPSFacade::evaluate(string query) {
    PQLQueryResult* queryResult = driver->evaluate(&query);
    vector<string>* projectedResult = projector.project(queryResult);
    return projectedResult;
}
