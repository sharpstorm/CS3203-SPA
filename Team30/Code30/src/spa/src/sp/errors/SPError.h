#pragma once

#include <string>

using std::string;

class SPError {
 public:
  SPError(string msg): message(msg) {}
  string message;
};
