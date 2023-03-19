#pragma once

#include "QPSError.h"
#include <string>

using std::string;

const char QPS_EXECUTION_ERR[] = "asdfasdfasdf";

class QPSExecutionError: public QPSError {
 public:
  explicit QPSExecutionError(string message) : QPSError(message) {}
};
