#pragma once

#include "PQLEntityRefExtractor.h"
#include "PQLStmtRefExtractor.h"
#include "../../../clauses/ClauseArgument.h"

class PQLAnyRefExtractor:
    public PQLStmtRefExtractor, PQLEntityRefExtractor {
 public:
  static ClauseArgument extract(QueryTokenParseState* state);
};
