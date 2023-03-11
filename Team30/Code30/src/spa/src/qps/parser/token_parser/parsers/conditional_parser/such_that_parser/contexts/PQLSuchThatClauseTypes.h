#pragma once

#include "qps/parser/token_parser/ref_extractor/PQLAnyRefExtractor.h"
#include "PQLSuchThatClauseContext.h"
#include "PQLTransitiveClauseContext.h"

#include "qps/clauses/such_that/UsesClause.h"
#include "qps/clauses/such_that/ModifiesClause.h"
#include "qps/clauses/such_that/CallsClauses.h"
#include "qps/clauses/such_that/FollowsClauses.h"
#include "qps/clauses/such_that/ParentClauses.h"
#include "qps/clauses/such_that/NextClauses.h"
#include "qps/clauses/such_that/AffectsClauses.h"

typedef PQLTransitiveClauseContext<FollowsClause,
                                   FollowsTClause,
                                   PQLStmtRefExtractor,
                                   PQLStmtRefExtractor>
    PQLFollowsClauseContext;

typedef PQLTransitiveClauseContext<ParentClause,
                                   ParentTClause,
                                   PQLStmtRefExtractor,
                                   PQLStmtRefExtractor>
    PQLParentClauseContext;

typedef PQLTransitiveClauseContext<CallsClause,
                                   CallsTClause,
                                   PQLEntityRefExtractor,
                                   PQLEntityRefExtractor>
    PQLCallsClauseContext;

typedef PQLSuchThatClauseContext<UsesClause,
                                 PQLAnyRefExtractor,
                                 PQLEntityRefExtractor>
    PQLUsesClauseContext;

typedef PQLSuchThatClauseContext<ModifiesClause,
                                 PQLAnyRefExtractor,
                                 PQLEntityRefExtractor>
    PQLModifiesClauseContext;

typedef PQLTransitiveClauseContext<NextClause,
                                   NextTClause,
                                   PQLStmtRefExtractor,
                                   PQLStmtRefExtractor>
    PQLNextClauseContext;

typedef PQLTransitiveClauseContext<AffectsClause,
                                   AffectsTClause,
                                   PQLStmtRefExtractor,
                                   PQLStmtRefExtractor>
    PQLAffectsClauseContext;
