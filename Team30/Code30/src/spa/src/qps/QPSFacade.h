#pragma once

#include <string>
#include <vector>
#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"

using std::string, std::vector;

class QPSFacade: public IQPS {
 private:
  IQueryDriver* driver;
  ResultProjector projector;

 public:
  QPSFacade(shared_ptr<PkbQueryHandler> pkbQH);
  ~QPSFacade();
  vector<string>* evaluate(string query);
};
