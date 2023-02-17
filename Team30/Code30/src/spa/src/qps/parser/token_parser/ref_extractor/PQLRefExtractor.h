#pragma once

#include <memory>
#include "../../../clauses/arguments/ClauseArgument.h"
#include "../QueryTokenParseState.h"

using std::unique_ptr;

class PQLRefExtractor {
 public:
  virtual ~PQLRefExtractor() = default;
 protected:
  static ClauseArgumentPtr
      extractCommonRef(QueryTokenParseState* state);
};
