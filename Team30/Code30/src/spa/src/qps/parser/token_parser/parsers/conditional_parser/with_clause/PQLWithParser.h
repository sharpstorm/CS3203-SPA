#pragma once

#include <memory>
#include <string>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"
#include "qps/clauses/WithClause.h"

using std::make_unique, std::string;

class PQLWithParser: public IPQLParser {
  static ConstraintSPtr parseConstraint(
      WithArgumentPtr left, WithArgumentPtr right, QueryBuilder* builder);
  static ConstraintSPtr handleConstant(WithArgumentPtr left,
                                       WithArgumentPtr right);
  static ConstraintSPtr handleOverride(WithArgumentPtr left,
                                       WithArgumentPtr right,
                                       QueryBuilder* builder);
  static ConstraintSPtr handleSameSyn(WithArgumentPtr left,
                                      WithArgumentPtr right);

  static ConstraintSPtr parseOverrideConstraint(
      WithArgumentPtr synArg, WithArgumentPtr staticArg);
  static void addWithSelectClause(QueryBuilder* builder,
                                  const AttributedSynonym &attrSyn,
                                  const string &identValue);

  static WithArgumentPtr parseWithArg(QueryTokenParseState* state,
                                      QueryBuilder* builder);
  static WithArgumentPtr processConstant(PQLToken *token);
  static bool isWithClause(WithArgument* left, WithArgument* right);

 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;
  static void parseWithClause(
      QueryTokenParseState *parserState, QueryBuilder *builder);
};
