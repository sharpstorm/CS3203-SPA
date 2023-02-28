#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/CallsClause.h"
#include "qps/clauses/such_that/CallsTClause.h"
#include "../../ref_extractor/PQLEntityRefExtractor.h"

class PQLCallsClauseContext :
    public PQLTransitiveClauseContext<CallsClause, CallsTClause> {
 public:
  void parse(QueryTokenParseState* parseState) {
    PQLTransitiveClauseContext::parseTransitive<
        PQLEntityRefExtractor, PQLEntityRefExtractor>(parseState);
  }
};
