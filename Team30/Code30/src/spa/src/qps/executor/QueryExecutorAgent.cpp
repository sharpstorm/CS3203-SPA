#include "QueryExecutorAgent.h"

QueryExecutorAgent::QueryExecutorAgent(PkbQueryHandler *pkb,
                                       OverrideTable *table):
    pkbQueryHandler(pkb), overrideTable(table) {}

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

void QueryExecutorAgent::addToNextTCache(StmtStmtQueryResult* result) {
  cacheAgent.addToNextTCache(result);
}

void QueryExecutorAgent::addToAffectsCache(StmtStmtQueryResult* result) {
  cacheAgent.addToAffectsCache(result);
}

StmtStmtQueryResultPtr QueryExecutorAgent::queryNextTCache(
    const StmtRef& left, const StmtRef& right) {
  return cacheAgent.queryNextTCache(left, right);
}

StmtStmtQueryResultPtr QueryExecutorAgent::queryAffectsCache(
    const StmtRef& left, const StmtRef& right) {
  return cacheAgent.queryAffectsCache(left, right);
}
