#pragma once

#include "PQLTransitiveClauseContext.h"
#include "../../../../clauses/FollowsClause.h"
#include "../../../../clauses/FollowsTClause.h"
#include "../../ref_extractor/PQLStmtRefExtractor.h"

class PQLFollowsClauseContext: public PQLTransitiveClauseContext<
    FollowsClause, FollowsTClause> {
 public:
  void parse(QueryTokenParseState* parserState) {
    PQLTransitiveClauseContext::parseTransitive<
        PQLStmtRefExtractor,
        PQLStmtRefExtractor>(parserState);
  }
};
