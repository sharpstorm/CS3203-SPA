#include "PkbStmtStmtQueryInvoker.h"

PkbStmtStmtQueryInvoker::PkbStmtStmtQueryInvoker(
    IStructureMappingProvider *provider, StmtPredicateFactory *predicateFactory)
    : BaseQueryInvoker(provider, provider, predicateFactory, predicateFactory) {
}
