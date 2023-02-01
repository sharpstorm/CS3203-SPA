#pragma once

#include <string>
#include "QPSError.h"

using std::string;

class QPSLexerError: public QPSError {
 public:
  explicit QPSLexerError(string message) : QPSError(message) {}
};
