#pragma once

#include <string>

class IIfPatternWriter {
 public:
  virtual ~IIfPatternWriter() {}
  virtual void addIfPattern(int stmt, std::string variable) = 0;
};
