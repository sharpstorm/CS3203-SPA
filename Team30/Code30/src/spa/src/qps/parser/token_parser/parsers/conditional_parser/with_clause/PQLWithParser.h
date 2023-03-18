#pragma once

#include <memory>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"
#include "qps/clauses/WithClause.h"

using std::make_unique;

class PQLWithParser: public IPQLParser {
  static ConstraintSPtr parseConstraint(
      WithArgumentPtr left, WithArgumentPtr right, QueryBuilder* builder);
  static ConstraintSPtr parseOverrideConstraint(
      WithArgumentPtr synArg, WithArgumentPtr staticArg);
  static bool isNonDefaultCase(AttributedSynonym attrSyn);
  static void addWithSelectClause(QueryBuilder* builder,
                                  AttributedSynonym attrSyn);
 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;
  static void parseWithClause(
      QueryTokenParseState *parserState, QueryBuilder *builder);
};
