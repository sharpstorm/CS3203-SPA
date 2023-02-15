#pragma once

#include "PQLRefExtractor.h"
#include "../../../clauses/ClauseArgument.h"

class PQLEntityRefExtractor: public PQLRefExtractor {
 public:
  static ClauseArgument extract(QueryTokenParseState* state);

 protected:
  static ClauseArgument extractEntity(QueryTokenParseState* state);
};
