#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/ParentClause.h"
#include "qps/clauses/such_that/ParentTClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLStmtRefExtractor.h"
#include "IPQLSuchThatClauseContext.h"

class PQLParentClauseContext:
    public PQLTransitiveClauseContext<ParentClause, ParentTClause>,
    public IPQLSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState* parserState,
                          QueryBuilder* builder) {
    return PQLTransitiveClauseContext::parseTransitive<
        PQLStmtRefExtractor,
        PQLStmtRefExtractor>(parserState, builder);
  }
};
