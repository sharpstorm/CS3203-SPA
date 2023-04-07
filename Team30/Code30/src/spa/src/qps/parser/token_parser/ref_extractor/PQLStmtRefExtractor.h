#pragma once

#include "PQLRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgument.h"

class PQLStmtRefExtractor : public PQLRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState *state,
                                   QueryBuilder *builder);

 protected:
  static ClauseArgumentPtr
  extractStatement(QueryTokenParseState *state,
                   QueryBuilder *builder);
};
