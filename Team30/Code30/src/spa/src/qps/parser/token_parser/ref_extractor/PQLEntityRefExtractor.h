#pragma once

#include "PQLRefExtractor.h"
#include "../../../clauses/arguments/ClauseArgument.h"

class PQLEntityRefExtractor: public PQLRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                   QueryBuilder* builder);

 protected:
  static ClauseArgumentPtr extractEntity(QueryTokenParseState* state);
};
