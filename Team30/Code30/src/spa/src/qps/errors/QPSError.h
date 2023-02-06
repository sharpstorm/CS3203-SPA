#pragma once

#include <string>

using std::string;

class QPSError {
 public:
  explicit QPSError(string message) { this->message = message; }
  string message;
};
