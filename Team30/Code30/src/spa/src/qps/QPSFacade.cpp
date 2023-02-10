#include "QPSFacade.h"

QPSFacade::QPSFacade(shared_ptr<PkbQueryHandler> pkbQH) {
  driver = new QueryDriver(pkbQH);
}

QPSFacade::~QPSFacade() noexcept {
  delete(driver);
}

UniqueVectorPtr<string> QPSFacade::evaluate(string query) {
    PQLQueryResult* queryResult = driver->evaluate(&query);
    UniqueVectorPtr<string> projectedResult = projector.project(queryResult);
    delete(queryResult);
    return projectedResult;
}
