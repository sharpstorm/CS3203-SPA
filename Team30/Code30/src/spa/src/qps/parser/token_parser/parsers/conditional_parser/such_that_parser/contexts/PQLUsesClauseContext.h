#pragma once

#include "AbstractSuchThatClauseContext.h"
#include "qps/clauses/such_that/UsesClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"
#include "qps/parser/token_parser/ref_extractor/PQLAnyRefExtractor.h"

class PQLUsesClauseContext: public AbstractSuchThatClauseContext,
                            public IPQLSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState* parserState,
                          QueryBuilder* builder) {
    return parseArgs<
        UsesClause,
        PQLAnyRefExtractor,
        PQLEntityRefExtractor>(parserState, builder);
  }
};
