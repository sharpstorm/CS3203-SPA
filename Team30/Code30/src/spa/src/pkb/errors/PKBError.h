#pragma once

#include <string>

#include "common/errors/AbstractError.h"

const char PKBERR_DUPLICATE_PROCEDURE[] = "Procedure already defined";
const char PKBERR_CYCLIC_PROCEDURE[] = "Cyclic calls detected";
const char PKBERR_NONEXISTENT_PROCEDURE[] = "Non-existent procedure called";

using std::string;

class PKBError : public AbstractError {
 public:
  explicit PKBError(const string &message) : AbstractError(message) {}
};
