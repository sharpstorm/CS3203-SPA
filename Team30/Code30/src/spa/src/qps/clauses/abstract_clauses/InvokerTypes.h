#pragma once

#include "common/Types.h"
#include "qps/clauses/such_that/AbstractSuchThatClause.h"

typedef QueryInvoker<EntityValue, EntityRef, EntityValue, EntityRef>
    EntEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, EntityValue, EntityRef>
    StmtEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, StmtValue, StmtRef>
    StmtStmtInvoker;
