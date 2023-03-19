#include "PkbStmtEntQueryInvoker.h"

PkbStmtEntQueryInvoker::PkbStmtEntQueryInvoker(
    IStructureMappingProvider *structureMappingProvider,
    StmtPredicateFactory *leftPredicateFactory,
    EntityPredicateFactory *rightPredicateFactory)
    : BaseQueryInvoker(
    structureMappingProvider,
    leftPredicateFactory,
    rightPredicateFactory) {}

