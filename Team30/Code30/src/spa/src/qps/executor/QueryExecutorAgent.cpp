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
void QueryExecutorAgent::addToNextTCache(QueryResult<StmtValue,
                                                     StmtValue> *result) {
  cacheAgent.addToNextTCache(result);
}

void QueryExecutorAgent::addToAffectsCache(QueryResult<StmtValue,
                                                       StmtValue> *result) {
  cacheAgent.addToAffectsCache(result);
}

void QueryExecutorAgent::addToAffectsTCache(QueryResult<StmtValue,
                                                        StmtValue> *result) {
  cacheAgent.addToAffectsTCache(result);
}

StmtStmtQueryResult QueryExecutorAgent::queryNextTCache(
    StmtRef left, StmtRef right) {
  return cacheAgent.queryNextTCache(left, right);
}

StmtStmtQueryResult QueryExecutorAgent::queryAffectsCache(
    StmtRef left, StmtRef right) {
  return cacheAgent.queryAffectsCache(left, right);
}

StmtStmtQueryResult QueryExecutorAgent::queryAffectsTCache(
    StmtRef left, StmtRef right) {
  return cacheAgent.queryAffectsTCache(left, right);
}
