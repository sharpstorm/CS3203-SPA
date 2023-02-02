#pragma once

#include <string>
#include <iostream>
#include "QPSError.h"

using std::string;

class QPSParserError: public QPSError {
 public:
  explicit QPSParserError(string message) : QPSError(message) {}
};
