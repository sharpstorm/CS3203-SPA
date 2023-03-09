#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"

class PQLWithParser: public IPQLParser {
//  PQLSynonymAttribute extractAttribute(QueryTokenParseState *parserState,
//                      QueryBuilder *builder, PQLSynonymType type);
 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;
//  PQLSynonymAttribute getSynAttribute(PQLToken *token);
  void parseWithClause(QueryTokenParseState *parserState,
                       QueryBuilder *builder);
};