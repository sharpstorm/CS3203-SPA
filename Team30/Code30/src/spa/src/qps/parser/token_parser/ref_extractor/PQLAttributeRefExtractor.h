#pragma once

#include "PQLStmtRefExtractor.h"
#include "PQLEntityRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"

class PQLAttributeRefExtractor :
    public PQLStmtRefExtractor, PQLEntityRefExtractor {
 public:
  static PQLSynonymAttribute extractAttribute(
      QueryTokenParseState *parserState);
  static WithArgumentPtr extract(QueryTokenParseState* state,
                                          QueryBuilder* builder);
};
