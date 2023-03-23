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
  static ConstraintSPtr handleTwoSyns(WithArgumentPtr left,
                                      WithArgumentPtr right,
                                      QueryBuilder* builder);

  static ConstraintSPtr parseOverrideConstraint(
      WithArgumentPtr synArg, WithArgumentPtr staticArg);
  static bool isNonDefaultCase(AttributedSynonym attrSyn);
  static void addWithSelectClause(QueryBuilder* builder,
                                  AttributedSynonym attrSyn,
                                  string identValue);

  static WithArgumentPtr parseWithArg(QueryTokenParseState* state,
                                      QueryBuilder* builder);
  static WithArgumentPtr PQLWithParser::processConstant(PQLToken *token);

 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;
  static void parseWithClause(
      QueryTokenParseState *parserState, QueryBuilder *builder);
};
