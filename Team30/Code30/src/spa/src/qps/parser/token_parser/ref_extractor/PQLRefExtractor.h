#pragma once

#include <memory>
#include "../../../clauses/arguments/ClauseArgument.h"
#include "../QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

using std::unique_ptr;

class PQLRefExtractor {
 public:
  virtual ~PQLRefExtractor() = default;
 protected:
  static ClauseArgumentPtr
      extractCommonRef(QueryTokenParseState* state, QueryBuilder* builder);
  PQLSynonymAttribute extractAttribute(QueryTokenParseState *parserState,
                                       QueryBuilder *builder,
                                       PQLSynonymType type);
  PQLSynonymAttribute getSynAttribute(PQLToken *token);
  ClauseArgumentPtr extractAttributeRef(QueryTokenParseState *state,
                                        QueryBuilder *builder);
};
