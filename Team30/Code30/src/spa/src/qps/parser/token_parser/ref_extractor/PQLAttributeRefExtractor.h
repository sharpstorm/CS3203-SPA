#pragma once

#include "PQLStmtRefExtractor.h"
#include "PQLEntityRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"

class PQLAttributeRefExtractor : public PQLRefExtractor {
  using ParsePredicate = bool(*)(QueryTokenParseState *state);
 public:
  static AttributedSynonymPtr extractOptionalAttr(QueryTokenParseState *state,
                                                  QueryBuilder *builder);
  static AttributedSynonymPtr extractCompulsoryAttr(QueryTokenParseState *state,
                                                    QueryBuilder *builder);

 private:
  template <ParsePredicate expectPredicate>
  static AttributedSynonymPtr extract(QueryTokenParseState *state,
                                      QueryBuilder *builder);
  static PQLSynonymAttribute extractAttribute(QueryTokenParseState *state);
  static bool expectAttribute(QueryTokenParseState *state);
  static bool tryExpectAttribute(QueryTokenParseState *state);
};
