#include <memory>

#include "QueryExecutorAgent.h"

using std::make_unique;

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
  for (auto it : result->pairVals) {
    nextTCache.addEntry(it.first, it.second);
  }
}

void QueryExecutorAgent::addToAffectsCache(StmtStmtQueryResult* result) {
  for (auto it : result->pairVals) {
    affectsCache.addEntry(it.first, it.second);
  }
}

StmtStmtQueryResultPtr QueryExecutorAgent::queryNextTCache(
    const StmtRef& left, const StmtRef& right) {
  StmtValue leftVal = left.getValue();
  StmtValue rightVal = right.getValue();

  CacheRow* result = nextTCache.queryFull(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  result = nextTCache.queryPartial(leftVal, rightVal);

  return toQueryResult(left, right, result);
}

StmtStmtQueryResultPtr QueryExecutorAgent::queryAffectsCache(
    const StmtRef& left, const StmtRef& right) {
  StmtValue leftVal = left.getValue();
  StmtValue rightVal = right.getValue();

  CacheRow* result = affectsCache.queryFull(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  result = affectsCache.queryPartial(leftVal, rightVal);

  return toQueryResult(left, right, result);
}

StmtStmtQueryResultPtr QueryExecutorAgent::toQueryResult(const StmtRef& left,
                                                         const StmtRef& right,
                                                         CacheRow *row) {
  StmtStmtQueryResultPtr result = make_unique<StmtStmtQueryResult>();

  if (row == nullptr || row->empty()) {
    return result;
  }

  // (static, static)
  if (left.isKnown() && right.isKnown()) {
    result->add(left.getValue(), right.getValue());
    return result;
  }

  // (syn, static)
  if (!left.isKnown()) {
    for (auto r : *row) {
      result->add(r, right.getValue());
    }
  }

  // (static, syn)
  if (!right.isKnown()) {
    for (auto r : *row) {
      result->add(left.getValue(), r);
    }
  }

  return result;
}

bool QueryExecutorAgent::entryExistsNextT(const StmtRef& left,
                                          const StmtRef& right) {
  return nextTCache.entryExists(left.getValue(), right.getValue());
}

bool QueryExecutorAgent::entryExistsAffects(const StmtRef& left,
                                            const StmtRef& right) {
  return affectsCache.entryExists(left.getValue(), right.getValue());
}
