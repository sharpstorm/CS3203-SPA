#pragma once

#include "common/errors/AbstractError.h"

class QPSError : public AbstractError {
 protected:
  explicit QPSError(const ErrorMessage &message) : AbstractError(message) {}
};
