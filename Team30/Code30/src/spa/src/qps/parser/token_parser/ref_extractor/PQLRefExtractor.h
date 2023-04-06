#pragma once

#include "../../../clauses/arguments/ClauseArgument.h"
#include "../QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

class PQLRefExtractor {
 public:
  virtual ~PQLRefExtractor() = default;

 protected:
  static ClauseArgumentPtr extractCommonRef(QueryTokenParseState *state,
                                            QueryBuilder *builder);
};
