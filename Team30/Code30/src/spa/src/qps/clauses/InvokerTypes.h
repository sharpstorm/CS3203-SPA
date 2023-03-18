#pragma once

#include <unordered_set>

#include "common/Types.h"
#include "qps/common/constraint/Constraint.h"

template <
    typename LeftResultType, typename LeftArgType,
    typename RightResultType, typename RightArgType>
using QueryInvoker = QueryResult<LeftResultType, RightResultType>(*)(
    PkbQueryHandler* pkbQueryHandler,
    const LeftArgType &leftArg,
    const RightArgType &rightArg);

template <typename ResultType, typename ArgType>
using SymmetricQueryInvoker = unordered_set<ResultType>(*)(
    PkbQueryHandler* pkbQueryHandler,
    const ArgType &arg);

template <typename ResultType>
using ArgumentTransformer = ResultType(*)(ClauseArgument* arg);

typedef QueryInvoker<EntityValue, EntityRef, EntityValue, EntityRef>
    EntEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, EntityValue, EntityRef>
    StmtEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, StmtValue, StmtRef>
    StmtStmtInvoker;

typedef SymmetricQueryInvoker<StmtValue, StmtRef> StmtInvoker;
typedef SymmetricQueryInvoker<EntityValue, EntityRef> EntInvoker;
