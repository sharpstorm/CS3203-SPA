#include "QPSFacade.h"

QPSFacade::QPSFacade(shared_ptr<PkbQueryHandler> pkbQH) {
  driver = new QueryDriver(pkbQH);
}

QPSFacade::~QPSFacade() noexcept {
  delete(driver);
}

UniqueVectorPtr<string> QPSFacade::evaluate(string query) {
  return driver->evaluate(&query);
}
