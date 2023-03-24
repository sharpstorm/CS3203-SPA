#pragma once

#include "PQLEntityRefExtractor.h"
#include "PQLStmtRefExtractor.h"
#include "../../../clauses/arguments/ClauseArgument.h"

class PQLAnyRefExtractor: public PQLStmtRefExtractor, PQLEntityRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                   QueryBuilder* builder);
};
