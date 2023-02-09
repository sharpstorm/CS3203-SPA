#pragma once

#include <string>
#include <vector>
#include <memory>
#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"

using std::string, std::vector;

class QPSFacade: public IQPS {
 private:
  IQueryDriver* driver;
  ResultProjector projector;

 public:
  explicit QPSFacade(shared_ptr<PkbQueryHandler> pkbQH);
  ~QPSFacade() noexcept;
  vector<string>* evaluate(string query);
};
