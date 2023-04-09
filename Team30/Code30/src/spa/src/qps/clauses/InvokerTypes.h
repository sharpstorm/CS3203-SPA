#pragma once

#include <memory>

#include "common/Types.h"
#include "../executor/QueryExecutorAgent.h"

using std::unique_ptr;

template <
    typename LeftResultType, typename LeftArgType,
    typename RightResultType, typename RightArgType>
using QueryInvoker = QueryResultPtr<LeftResultType, RightResultType>(*)(
    const QueryExecutorAgent &agent,
    const LeftArgType &leftArg,
    const RightArgType &rightArg);

template <typename ResultType, typename ArgType>
using SymmetricQueryInvoker = QueryResultSet<ResultType>(*)(
    const QueryExecutorAgent &agent,
    const ArgType &arg);

template <typename ResultType>
using ArgumentTransformer = ResultType(*)(const ClauseArgument* arg);

typedef QueryInvoker<EntityValue, EntityRef, EntityValue, EntityRef>
    EntEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, EntityValue, EntityRef>
    StmtEntInvoker;

typedef QueryInvoker<StmtValue, StmtRef, StmtValue, StmtRef>
    StmtStmtInvoker;

typedef SymmetricQueryInvoker<StmtValue, StmtRef> StmtInvoker;
typedef SymmetricQueryInvoker<EntityValue, EntityRef> EntInvoker;
