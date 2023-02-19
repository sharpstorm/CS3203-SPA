#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/clauses/such_that/FollowsTClause.h"
#include "../../ref_extractor/PQLStmtRefExtractor.h"

class PQLFollowsClauseContext:
    public PQLTransitiveClauseContext<FollowsClause, FollowsTClause> {
 public:
  void parse(QueryTokenParseState* parserState) {
    PQLTransitiveClauseContext::parseTransitive<
        PQLStmtRefExtractor,
        PQLStmtRefExtractor>(parserState);
  }
};
