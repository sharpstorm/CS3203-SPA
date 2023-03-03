#include "QPSFacade.h"

QPSFacade::QPSFacade(PkbQueryHandler* pkbQH) {
  driver = new QueryDriver(pkbQH);
}

QPSFacade::~QPSFacade() noexcept {
  delete(driver);
}

UniqueVectorPtr<string> QPSFacade::evaluate(string query) {
  return driver->evaluate(&query);
}
