#pragma once

#include <memory>
#include "PQLEntityRefExtractor.h"
#include "PQLStmtRefExtractor.h"
#include "../../../clauses/arguments/ClauseArgument.h"

using std::unique_ptr;

class PQLAnyRefExtractor:
    public PQLStmtRefExtractor, PQLEntityRefExtractor {
 public:
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                   QueryBuilder* builder);
  static ClauseArgumentPtr extractAttr(QueryTokenParseState *state,
                                QueryBuilder *builder);
};
