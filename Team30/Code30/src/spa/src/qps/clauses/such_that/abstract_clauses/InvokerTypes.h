#pragma once

#include "common/Types.h"
#include "AbstractSuchThatClause.h"

typedef QueryInvoker<EntityValue, EntityRef, EntityValue, EntityRef>
    EntEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, EntityValue, EntityRef>
    StmtEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, StmtValue, StmtRef>
    StmtStmtInvoker;
