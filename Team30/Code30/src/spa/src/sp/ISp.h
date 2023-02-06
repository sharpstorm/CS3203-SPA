#pragma once
#include <string>

using std::string;

class ISp {
 public:
  virtual void parseSource(string input) = 0;
};
