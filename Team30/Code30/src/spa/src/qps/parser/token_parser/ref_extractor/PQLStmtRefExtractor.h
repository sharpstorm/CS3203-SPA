#pragma once

#include <memory>
#include "PQLRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgument.h"

using std::unique_ptr;

class PQLStmtRefExtractor: public PQLRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                   QueryBuilder* builder);

 protected:
  static ClauseArgumentPtr
      extractStatement(QueryTokenParseState* state,
                       QueryBuilder* builder);
};
