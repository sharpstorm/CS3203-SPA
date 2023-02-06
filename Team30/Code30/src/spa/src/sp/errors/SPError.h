#pragma once

#include <string>

using std::string;

class SPError {
 public:
  explicit SPError(string msg): message(msg) {}
  string message;
};
