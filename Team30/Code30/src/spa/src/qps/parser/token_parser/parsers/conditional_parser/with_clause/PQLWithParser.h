#pragma once

#include <memory>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"
#include "qps/clauses/WithClause.h"

using std::make_unique;

class PQLWithParser: public IPQLParser {
 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;
  unique_ptr<WithClause> parseWithClause(QueryTokenParseState *parserState,
                       QueryBuilder *builder);
};
