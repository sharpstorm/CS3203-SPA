#pragma once

#include "qps/parser/token_parser/ref_extractor/PQLAnyRefExtractor.h"
#include "PQLSuchThatClauseContext.h"
#include "PQLTransitiveClauseContext.h"

#include "qps/clauses/such_that/UsesClause.h"
#include "qps/clauses/such_that/ParentClause.h"
#include "qps/clauses/such_that/ParentTClause.h"
#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/clauses/such_that/FollowsTClause.h"
#include "qps/clauses/such_that/CallsClause.h"
#include "qps/clauses/such_that/CallsTClause.h"
#include "qps/clauses/such_that/ModifiesClause.h"

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
