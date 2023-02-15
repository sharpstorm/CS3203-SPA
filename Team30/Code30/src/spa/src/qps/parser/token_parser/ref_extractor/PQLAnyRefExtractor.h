#pragma once

#include "PQLEntityRefExtractor.h"
#include "PQLStmtRefExtractor.h"

class PQLAnyRefExtractor:
    public PQLStmtRefExtractor, PQLEntityRefExtractor {
 public:
  static ClauseArgument extract(QueryTokenParseState* state);
};
