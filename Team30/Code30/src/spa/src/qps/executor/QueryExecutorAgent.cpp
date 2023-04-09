#include <memory>

#include "QueryExecutorAgent.h"

using std::make_unique;

QueryExecutorAgent::QueryExecutorAgent(const PkbQueryHandler *pkb,
                                       const OverrideTable *table,
                                       QueryCache *cache) :
    pkbQueryHandler(pkb), overrideTable(table), cache(cache) {}

const PkbQueryHandler *QueryExecutorAgent::operator->() const {
  return pkbQueryHandler;
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
  if (!ref.isValid()) {
    return false;
  }

  if (ref.isType(StmtType::Wildcard) || !ref.isKnown()) {
    return true;
  }

  const StmtType stmtType = pkbQueryHandler->getStatementType(ref.getValue());
  if (ref.isType(StmtType::None)) {
    return stmtType != StmtType::None;
  }

  return stmtType == ref.getType();
}

bool QueryExecutorAgent::isValid(const EntityRef &ref) const {
  if (ref.isType(EntityType::None) || ref.isType(EntityType::Wildcard)) {
    return true;
  }

  return !ref.isKnown() || pkbQueryHandler->isSymbolOfType(ref.getType(),
                                                           ref.getValue());
}

CacheTable *QueryExecutorAgent::getAffectsCache() const {
  return cache->getAffectsCache();
}

const PkbQueryHandler *QueryExecutorAgent::getPkbQueryHandler() const {
  return pkbQueryHandler;
}
