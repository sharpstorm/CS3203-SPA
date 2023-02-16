#pragma once

#include "../../../clauses/ClauseArgument.h"
#include "../QueryTokenParseState.h"

class PQLRefExtractor {
 public:
  virtual ~PQLRefExtractor() = default;
 protected:
  static ClauseArgument extractCommonRef(QueryTokenParseState* state);
};
