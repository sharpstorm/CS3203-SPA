#include "AbstractError.h"

AbstractError::AbstractError(const ErrorMessage &message) : errMsg(message) {}

const char *AbstractError::what() const noexcept {
  return errMsg.c_str();
}
