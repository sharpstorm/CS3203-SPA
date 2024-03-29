#pragma once

#include <memory>

#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"
#include "qps/clauses/WithClause.h"

using std::make_unique;

class PQLWithParser : public IPQLParser {
  static void handleConstant(WithArgumentPtr left,
                             WithArgumentPtr right,
                             QueryBuilder *builder);

  static void handleOverride(WithArgumentPtr left,
                             WithArgumentPtr right,
                             QueryBuilder *builder);
  static void handleTwoSyns(WithArgumentPtr left,
                            WithArgumentPtr right,
                            QueryBuilder *builder);

  static ConstraintPtr parseOverrideConstraint(
      WithArgumentPtr synArg, WithArgumentPtr staticArg);

  static WithArgumentPtr parseWithArg(QueryTokenParseState *state,
                                      QueryBuilder *builder);
  static WithArgumentPtr processConstant(const PQLToken *token);

 public:
  void parse(QueryTokenParseState *parserState,
             QueryBuilder *builder) override;
  static void parseWithClause(
      QueryTokenParseState *parserState, QueryBuilder *builder);
};
