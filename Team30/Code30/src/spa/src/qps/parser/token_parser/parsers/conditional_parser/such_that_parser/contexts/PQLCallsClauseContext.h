#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/CallsClause.h"
#include "qps/clauses/such_that/CallsTClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"

class PQLCallsClauseContext :
    public PQLTransitiveClauseContext<CallsClause, CallsTClause>,
    public IPQLSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState* parseState,
                          QueryBuilder* builder) {
    return PQLTransitiveClauseContext::parseTransitive<
        PQLEntityRefExtractor, PQLEntityRefExtractor>(parseState, builder);
  }
};
