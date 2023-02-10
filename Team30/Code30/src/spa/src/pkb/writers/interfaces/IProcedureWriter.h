#pragma once

#include <string>

using std::string;

class IProcedureWriter {
 public:
  virtual void addProcedure(string, int, int) = 0;
};
