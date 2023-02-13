#pragma once

#include <string>
#include <memory>
#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"
#include "common/UtilityTypes.h"

using std::string;

class QPSFacade: public IQPS {
 private:
  IQueryDriver* driver;

 public:
  explicit QPSFacade(shared_ptr<PkbQueryHandler> pkbQH);
  ~QPSFacade() noexcept;
  UniqueVectorPtr<string> evaluate(string query);
};
