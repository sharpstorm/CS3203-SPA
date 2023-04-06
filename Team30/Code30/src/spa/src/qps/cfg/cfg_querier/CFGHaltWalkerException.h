#pragma once

#include "common/errors/AbstractError.h"

class CFGHaltWalkerException : public AbstractError {
 public:
  CFGHaltWalkerException() : AbstractError("CFG Walker Halted") {}
};
