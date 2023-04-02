#include <memory>

#include "QueryExecutorAgent.h"

using std::make_unique;

QueryExecutorAgent::QueryExecutorAgent(PkbQueryHandler *pkb,
                                       OverrideTable *table,
                                       QueryCache *cache):
    pkbQueryHandler(pkb), overrideTable(table), cache(cache) {}

PkbQueryHandler *QueryExecutorAgent::operator->() const {
  return this->pkbQueryHandler;
}

StmtRef QueryExecutorAgent::transformArg(const PQLSynonymName name,
                                         const StmtRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transformArg(name, ref);
}

EntityRef QueryExecutorAgent::transformArg(const PQLSynonymName name,
                                           const EntityRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transformArg(name, ref);
}

bool QueryExecutorAgent::isValid(const StmtRef &ref) const {
  if (ref.isType(StmtType::None)) {
    return true;
  }

  return !ref.isKnown()
      || pkbQueryHandler->isStatementOfType(ref.getType(), ref.getValue());
}

bool QueryExecutorAgent::isValid(const EntityRef &ref) const {
  if (ref.isType(EntityType::None)) {
    return true;
  }

  return !ref.isKnown() ||
      pkbQueryHandler->isSymbolOfType(ref.getType(), ref.getValue());
}

CacheTable* QueryExecutorAgent::getAffectsCache() const {
  return cache->getAffectsCache();
}
