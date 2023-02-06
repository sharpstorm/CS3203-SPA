#pragma once

#include <string>
#include <vector>
#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"

using std::string, std::vector;

class QPSFacade: public IQPS {
    IQueryDriver* driver;
    ResultProjector projector;
 public:
  explicit QPSFacade(IQueryDriver* driver);
  vector<string>* evaluate(string query);
};
