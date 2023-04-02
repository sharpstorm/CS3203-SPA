#include "PkbStmtEntQueryInvoker.h"

PkbStmtEntQueryInvoker::PkbStmtEntQueryInvoker(
    IStructureMappingProvider *structureMappingProvider,
    IEntityMappingProvider *entityMappingProvider,
    StmtPredicateFactory *leftPredicateFactory,
    EntityPredicateFactory *rightPredicateFactory)
    : BaseQueryInvoker(structureMappingProvider, entityMappingProvider,
                       leftPredicateFactory, rightPredicateFactory) {}
