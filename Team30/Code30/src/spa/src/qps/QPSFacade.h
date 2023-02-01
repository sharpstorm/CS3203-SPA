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
  string* evaluate(string query);
};
