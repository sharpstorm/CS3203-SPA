#pragma once

#include <string>

using std::string;

class IQPS {
 public:
  virtual ~IQPS() = default;
  virtual string* evaluate(string query) = 0;
};
