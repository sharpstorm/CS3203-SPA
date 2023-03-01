#pragma once

#include <string>

using std::string;

class ICallsWriter {
 public:
  virtual ~ICallsWriter() {}
  virtual void addCalls(int arg1, string arg2, string arg3) = 0;
};
