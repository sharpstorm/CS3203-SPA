#pragma once

#include "PQLTransitiveClauseContext.h"
#include "../../../../clauses/ParentClause.h"
#include "../../../../clauses/ParentTClause.h"
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
