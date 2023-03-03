#include "QPSFacade.h"

QPSFacade::QPSFacade(PkbQueryHandler* pkbQH,
                     ISourceExpressionParser* exprParser) {
  driver = new QueryDriver(pkbQH, exprParser);
}

QPSFacade::~QPSFacade() noexcept {
  delete(driver);
}

UniqueVectorPtr<string> QPSFacade::evaluate(string query) {
  return driver->evaluate(&query);
}
