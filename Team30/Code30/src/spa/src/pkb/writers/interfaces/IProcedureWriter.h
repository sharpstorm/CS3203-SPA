#pragma once

#include <string>

using std::string;

class IProcedureWriter {
  virtual void addProcedure(string, int, int) = 0;
};
