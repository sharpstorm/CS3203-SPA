#pragma once

#include <string>

#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"
#include "common/UtilityTypes.h"
#include "common/parser/ISourceExpressionParser.h"

using std::string;

class QPSFacade: public IQPS {
 private:
  IQueryDriver* driver;

 public:
  explicit QPSFacade(PkbQueryHandler* pkbQH,
                     ISourceExpressionParser* exprParser);
  ~QPSFacade() noexcept;
  UniqueVectorPtr<string> evaluate(string query);
};
