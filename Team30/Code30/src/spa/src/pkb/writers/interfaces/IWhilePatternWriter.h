#pragma once

#include <string>

class IWhilePatternWriter {
 public:
  virtual ~IWhilePatternWriter() {}
  virtual void addWhilePattern(int stmt, std::string variable) = 0;
};
