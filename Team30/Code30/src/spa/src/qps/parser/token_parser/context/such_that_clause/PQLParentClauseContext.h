#pragma once

#include "PQLTransitiveClauseContext.h"
#include "qps/clauses/such_that/ParentClause.h"
#include "qps/clauses/such_that/ParentTClause.h"
#include "../../ref_extractor/PQLStmtRefExtractor.h"

class PQLParentClauseContext:
    public PQLTransitiveClauseContext<ParentClause, ParentTClause> {
 public:
  void parse(QueryTokenParseState* parserState) {
    PQLTransitiveClauseContext::parseTransitive<
        PQLStmtRefExtractor,
        PQLStmtRefExtractor>(parserState);
  }
};
