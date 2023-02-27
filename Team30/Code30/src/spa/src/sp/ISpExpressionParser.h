#pragma once

#include <string>

using std::string;

class ISpExpressionParser {
 public:
  virtual ~ISpExpressionParser() = default;
  virtual void parseSource(string expression) = 0;
};
