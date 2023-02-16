#pragma once

#include "PQLRefExtractor.h"

class PQLStmtRefExtractor: public PQLRefExtractor {
 public:
  static ClauseArgument extract(QueryTokenParseState* state);

 protected:
  static ClauseArgument extractStatement(QueryTokenParseState* state);
};
