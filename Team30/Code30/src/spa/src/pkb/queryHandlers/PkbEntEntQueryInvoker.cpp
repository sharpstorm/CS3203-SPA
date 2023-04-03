#include "PkbEntEntQueryInvoker.h"

PkbEntEntQueryInvoker::PkbEntEntQueryInvoker(
    IEntityMappingProvider *provider, EntityPredicateFactory *predicateFactory)
    : BaseQueryInvoker(provider, provider, predicateFactory, predicateFactory) {
}
