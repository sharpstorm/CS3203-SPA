#pragma once

#include "PQLStmtRefExtractor.h"
#include "PQLEntityRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"

class PQLAttributeRefExtractor: public PQLRefExtractor {
 public:
  static AttributedSynonymPtr extract(QueryTokenParseState* state,
                                      QueryBuilder* builder);

 private:
  static PQLSynonymAttribute extractAttribute(QueryTokenParseState* state);
};
