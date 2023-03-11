#pragma once

#include "common/Types.h"

template <
    typename LeftResultType, typename LeftArgType,
    typename RightResultType, typename RightArgType>
using QueryInvoker = QueryResult<LeftResultType, RightResultType>(*)(
    PkbQueryHandler* pkbQueryHandler,
    const LeftArgType &leftArg,
    const RightArgType &rightArg);

template <typename ResultType>
using ArgumentTransformer = ResultType(*)(ClauseArgument* arg);

typedef QueryInvoker<EntityValue, EntityRef, EntityValue, EntityRef>
    EntEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, EntityValue, EntityRef>
    StmtEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, StmtValue, StmtRef>
    StmtStmtInvoker;
