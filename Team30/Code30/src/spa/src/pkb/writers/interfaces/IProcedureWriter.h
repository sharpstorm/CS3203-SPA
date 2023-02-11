#pragma once

#include <string>

using std::string;

class IProcedureWriter {
 public:
  virtual ~IProcedureWriter() {};
  virtual void addProcedure(string, int, int) = 0;
};
