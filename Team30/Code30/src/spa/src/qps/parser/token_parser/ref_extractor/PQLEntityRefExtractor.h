#pragma once

#include <memory>
#include "PQLRefExtractor.h"
#include "../../../clauses/arguments/ClauseArgument.h"

using std::unique_ptr;

class PQLEntityRefExtractor: public PQLRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                   QueryBuilder* builder);

 protected:
  static ClauseArgumentPtr extractEntity(QueryTokenParseState* state);
};
