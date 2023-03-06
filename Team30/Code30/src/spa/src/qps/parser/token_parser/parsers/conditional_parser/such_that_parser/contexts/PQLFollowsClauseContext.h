#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/clauses/such_that/FollowsTClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLStmtRefExtractor.h"
#include "IPQLSuchThatClauseContext.h"

class PQLFollowsClauseContext:
    public PQLTransitiveClauseContext<FollowsClause, FollowsTClause>,
    public IPQLSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState* parserState,
                          QueryBuilder* builder) {
    return PQLTransitiveClauseContext::parseTransitive<
        PQLStmtRefExtractor,
        PQLStmtRefExtractor>(parserState, builder);
  }
};
