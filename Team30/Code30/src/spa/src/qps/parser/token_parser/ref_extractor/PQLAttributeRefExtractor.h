#pragma once

#include "PQLStmtRefExtractor.h"
#include "PQLEntityRefExtractor.h"

class PQLAttributeRefExtractor :
    public PQLStmtRefExtractor, PQLEntityRefExtractor {
  static PQLSynonymAttribute getSynAttribute(PQLToken *token);
 public:
  static PQLSynonymAttribute extractAttribute(QueryTokenParseState *parserState,
                                              PQLSynonymType type);
  static ClauseArgumentPtr extract(QueryTokenParseState* state,
                                          QueryBuilder* builder);
};
