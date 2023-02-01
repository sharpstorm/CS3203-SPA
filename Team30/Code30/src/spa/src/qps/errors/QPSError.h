#pragma once

#include <string>

using std::string;

class QPSError {
 public:
  explicit QPSError(string message) { message = message; }
  string message;
};
