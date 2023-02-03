#pragma once

#include <string>

using std::string;

class QueryBuilderError {
 public:
  explicit QueryBuilderError(string message) { this->message = message; }
  string message;
};
