#pragma once

#include <string>
#include "IQPS.h"

using std::string;

class QPSFacade: public IQPS {
 public:
  string* evaluate(string query);
};
