#pragma once

#include <string>
#include "common/errors/AbstractError.h"

using std::string;

class QPSError : public AbstractError {
 protected:
  explicit QPSError(const string &message) : AbstractError(message) {}
};
