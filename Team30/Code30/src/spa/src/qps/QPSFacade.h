#pragma once

#include <string>
#include "IQPS.h"
#include "qps/projector/ResultProjector.h"
#include "QueryDriver.h"

using std::string;

class QPSFacade: public IQPS {
    IQueryDriver driver;
    ResultProjector projector;
 public:
  explicit QPSFacade(IQueryDriver driver);
  string* evaluate(string query);
};
