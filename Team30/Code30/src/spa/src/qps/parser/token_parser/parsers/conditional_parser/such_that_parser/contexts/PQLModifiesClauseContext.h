#pragma once

#include "AbstractSuchThatClauseContext.h"
#include "qps/clauses/such_that/ModifiesClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"
#include "qps/parser/token_parser/ref_extractor/PQLAnyRefExtractor.h"

class PQLModifiesClauseContext:
    public AbstractSuchThatClauseContext,
    public IPQLSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState* parserState,
                          QueryBuilder* builder) {
    return parseArgs<
        ModifiesClause,
        PQLAnyRefExtractor,
        PQLEntityRefExtractor>(parserState, builder);
  }
};
