#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

class IQPS {
 public:
  virtual ~IQPS() = default;
  virtual vector<string>* evaluate(string query) = 0;
};
